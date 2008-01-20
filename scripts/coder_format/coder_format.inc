<?php
// $Id$


/**
 * Recursively process .module and .inc files in directory with coder_format_file().
 *
 * @param $root
 *   Path to root directory for Drupal installation.
 * @param $undo
 *   Boolean whether or not to undo batch replacements.
 * @param $file_inc
 *   Path to Drupal's file.inc, default is $root/includes/file.inc
 */
function coder_format_recursive($root, $undo = false, $file_inc = null) {
  // Convert Windows paths.
  $root = str_replace("\\", '/', $root);

  // Check if directory exists.
  if (!file_exists($root)) {
    echo 'ERROR: '. $root .' does not exist!';
    return;
  }

  // Trim root directory.
  if (substr($root, -1) == '/') {
    $root = substr($root, 0, -1);
  }

  // Include Drupal's file.inc.
  if (!isset($file_inc)) {
    if (!file_exists($root .'/includes/file.inc')) {
      echo 'ERROR: '. $root .'/includes/file.inc not found!';
      return;
    }
  }
  else {
    if (!file_exists($file_inc)) {
      echo 'ERROR: '. $file_inc .' not found!';
      return;
    }
  }
  require_once (!isset($file_inc) ? $root .'/includes/file.inc' : $file_inc);

  if (!$undo) {
    // Fetch files to process.
    $mask   = '\.php$|\.module$|\.inc$|\.install|\.profile$';
    $nomask = array('.', '..', 'CVS', '.svn');
    $files  = file_scan_directory($root, $mask, $nomask, 0, true);
    foreach ($files as $file) {
      // file_copy() assigns $file->filename by reference.
      $sourcefile = $file->filename;
      echo $file->filename ."\n";
      file_copy($file->filename, $file->filename .'.coder.orig');
      coder_format_file($sourcefile, $sourcefile);
    }
  }
  else {
    // Fetch files to process.
    $mask   = '.coder.orig$';
    $nomask = array('.', '..', 'CVS', '.svn');
    $files  = file_scan_directory($root, $mask, $nomask, 0, true);
    foreach ($files as $file) {
      $file->origname = str_replace('.coder.orig', '', $file->filename);
      echo $file->origname ."\n";
      file_move($file->filename, $file->origname, FILE_EXISTS_REPLACE);
    }
  }
}

/**
 * Reads, processes and writes the source code from and to a file.
 *
 * @param $sourcefile
 *   Path to source file to read code from.
 * @param $targetfile
 *   Target path to write formatted source code to.
 *
 * @return
 *   TRUE if successful.
 */
function coder_format_file($sourcefile = null, $targetfile = null) {
  if (!isset($sourcefile) || !isset($targetfile)) {
    return;
  }

  // Read source code from source file.
  $fd = fopen($sourcefile, 'r');
  $code = fread($fd, filesize($sourcefile));
  fclose($fd);

  if ($code !== false) {
    $code = coder_format_string_all($code);

    if ($code !== false) {
      // Write formatted source code to target file.
      $fd = fopen($targetfile, 'w');
      $status = fwrite($fd, $code);
      fclose($fd);
      return $status;
    }
  }

  return false;
}

/**
 * Formats source code according to Drupal conventions, also using
 * post and pre-processors.
 *
 * @param
 *   $code Code to process.
 */
function coder_format_string_all($code) {
  // Preprocess source code.
  $code = coder_exec_processors($code, 'coder_preprocessor');

  // Process source code.
  $code = coder_format_string($code);

  // Postprocess source code.
  $code = coder_exec_processors($code, 'coder_postprocessor');

  // Fix beginning and end of code.
  $code = coder_trim_php($code);

  return $code;
}

/**
 * Format the source code according to Drupal coding style guidelines.
 *
 * This function uses PHP's tokenizer functions.
 * @see <http://www.php.net/manual/en/ref.tokenizer.php>
 *
 * To achieve the desired coding style, we have to take some special cases
 * into account. These are:
 *
 * Indent-related:
 *   $_coder_indent int Indent level
 *      The number of indents for the next line. This is
 *      - increased after {, : (after case and default).
 *      - decreased after }, break, case and default (after a previous case).
 *   $in_case bool
 *      Is true after case and default. Is false after break and return, if
 *      $braces_in_case is not greater than 0.
 *   $braces_in_case int Count of braces
 *      The number of currently opened curly braces in a case. This is needed
 *      to support arbitrary function exits inside of a switch control strucure.
 *   $parenthesis int Parenthesis level
 *      The number of currently opened parenthesis. This
 *      - prevents line feeds in brackets (f.e. in arguments of for()).
 *      - is the base for formatting of multiline arrays. Note: If the last
 *        ');' is not formatted to the correct indent level then there is no
 *        ',' (comma) behind the last array value.
 *   $in_brace bool
 *      Is true after left curly braces if they are in quotes, an object or
 *      after a dollar sign. Prevents line breaks around such variable
 *      statements.
 *   $in_heredoc bool
 *      Is true after heredoc output method and false after heredoc delimiter.
 *      Prevents line breaks in heredocs.
 *   $first_php_tag bool
 *      Is false after the first PHP tag. Allows inserting a line break after
 *      the first one.
 *   $in_do_while bool
 *      Is true after a do {} statement and set to false in the next while
 *      statement. Prevents a line break in the do {...} while() construct.
 *
 * Whitespace-related:
 *   $in_object bool
 *      Prevents whitespace after ->.
 *      Is true after ->. Is reset to false after the next string or variable.
 *   $in_at bool
 *      Prevents whitespace after @.
 *      Is true after @. Is reset to false after the next string or variable.
 *   $in_quote bool
 *      Prevents
 *      - removal of whitespace in double quotes.
 *      - injection of new line feeds after brackets in double quotes.
 *   $inline_if bool
 *      Controls formatting of ? and : for inline ifs until a ; (semicolon) is
 *      processed.
 *   $in_function_declaration
 *      Prevents whitespace after & for function declarations, e.g.
 *      function &foo(). Is true after function token but before first
 *      parenthesis.
 *   $in_array
 *      Array of parenthesis level to whether or not the structure
 *      is for an array.
 *   $in_multiline
 *      Array of parenthesis level to whether or not the structure
 *      is multiline.
 *   $after_semicolon
 *      Whether or not the current line being processed has a semicolon.
 *   $after_case
 *      Whether or not the current line being processed has a case/default statement.
 *
 * @param $code
 *      The source code to format.
 *
 * @return
 *      The formatted code or false if it fails.
 */
function coder_format_string($code = '') {
  global $_coder_indent;

  // Indent controls:
  $_coder_indent  = 0;
  $in_case        = false;
  $parenthesis    = 0;
  $braces_in_case = 0;
  $in_brace       = false;
  $in_heredoc     = false;
  $first_php_tag  = true;
  $in_do_while    = false;

  // Whitespace controls:
  $in_object       = FALSE;
  $in_at           = FALSE;
  $in_php          = FALSE;
  $in_quote        = FALSE;
  $inline_if       = FALSE;
  $in_array        = array();
  $in_multiline    = array();
  $after_semicolon = FALSE;
  $after_case      = FALSE;

  // Whether or not a function token was encountered:
  $in_function_declaration = FALSE;

  $result = '';
  $lasttoken = array(0);
  $tokens = token_get_all($code);

  // Mask T_ML_COMMENT (PHP4) as T_COMMENT (PHP5).
  // Mask T_DOC_COMMENT (PHP5) as T_ML_COMMENT (PHP4).
  if (!defined('T_ML_COMMENT')) {
    define('T_ML_COMMENT', T_COMMENT);
  }
  else if (!defined('T_DOC_COMMENT')) {
    define('T_DOC_COMMENT', T_ML_COMMENT);
  }

  foreach ($tokens as $token) {
    if (is_string($token)) {
      // Simple 1-character token.
      $text = trim($token);
      switch ($text) {
        case '{':
          // Write curly braces at the end of lines followed by a line break if
          // not in quotes (""), object ($foo->{$bar}) or in variables (${foo}).
          // (T_DOLLAR_OPEN_CURLY_BRACES exists but is never assigned.)
          if (!$in_variable && !$in_quote && !$in_object && substr(rtrim($result), -1) != '$') {
            if ($in_case) {
              ++$braces_in_case;
            }
            ++$_coder_indent;
            $result = rtrim($result) .' '. $text;
            coder_br($result);
          }
          else {
            $in_brace = true;
            $result .= $text;
          }
          break;

        case '}':
          if (!$in_quote && !$in_brace && !$in_heredoc) {
            if ($in_case) {
              --$braces_in_case;
            }
            --$_coder_indent;
            if ($braces_in_case < 0) {
              // Decrease indent if last case in a switch is not terminated.
              --$_coder_indent;
              $in_case = false;
              $braces_in_case = 0;
            }
            $result = rtrim($result);
            if (substr($result, -1) != '{') {
              // Avoid line break in empty curly braces.
              coder_br($result);
            }
            $result .= $text;
            coder_br($result);
          }
          else {
            $in_brace = false;
            $result .= $text;
          }
          break;

        case ';':
          $result = rtrim($result) . $text;
          if (!$parenthesis && !$in_heredoc) {
            coder_br($result);
            $after_semicolon = TRUE;
          }
          else {
            $result .= ' ';
          }
          if ($inline_if) {
            $inline_if = false;
          }
          break;

        case '?':
          $inline_if = true;
          $result .= ' '. $text .' ';
          break;

        case ':':
          if ($inline_if) {
            $result .= ' '. $text .' ';
          }
          else {
            if ($in_case) {
              ++$_coder_indent;
            }
            $result = rtrim($result) . $text;
            coder_br($result);
          }
          break;

        case '(':
          $result .= $text;
          ++$parenthesis;
          // Not multiline until proven so by whitespace.
          $in_multiline[$parenthesis] = FALSE;
          // If the $in_array flag for this parenthesis level was not
          // set previously, set it to FALSE.
          if (!isset($in_array[$parenthesis])) {
            $in_array[$parenthesis] = FALSE;
          }
          // Terminate function declaration, as a parenthesis indicates
          // the beginning of the arguments. This will catch all other
          // instances of parentheses, but in this case it's not a problem.
          $in_function_declaration = FALSE;
          break;

        case ')':
          if ($in_array[$parenthesis] && $in_multiline[$parenthesis]) {
            // Check if a comma insertion is necessary:
            for ($c = strlen($result) - 1; $c >= 0; $c--) {
              if ($result[$c] === "\n" || $result[$c] === " ") {
                continue;
              }
              if ($result[$c] === ",") {
                break;
              }
              // We need to add a comma at $c:
              $result = substr($result, 0, $c + 1) .','. substr($result, $c + 1);
              break;
            }
          }
          if (!$in_quote && !$in_heredoc && (substr(rtrim($result), -1) == ',' || $in_multiline[$parenthesis])) {
            // Fix indent of right parenthesis in multiline structures by
            // increasing indent for each parenthesis and decreasing one level.
            $_coder_indent = $_coder_indent + ($parenthesis - 1);
            $result = rtrim($result);
            coder_br($result);
            $result .= $text;
            // Undo temporary change.
            $_coder_indent = $_coder_indent - ($parenthesis - 1);
          }
          else {
            $result .= $text;
          }
          if ($parenthesis) {
            // Current parenthesis level is not an array anymore.
            $in_array[$parenthesis] = FALSE;
            --$parenthesis;
          }
          break;

        case '@':
          $in_at = true;
          $result .= $text;
          break;

        case ',':
          $result .= $text .' ';
          break;

        case '.':
          if (substr(rtrim($result), -1) == "'" || substr(rtrim($result), -1) == '"') {
            // Write string concatenation character directly after strings.
            $result = rtrim($result) . $text .' ';
          }
          else {
            $result = rtrim($result) .' '. $text .' ';
          }
          break;

        case '=':
        case '<':
        case '>':
        case '+':
        case '*':
        case '/':
        case '|':
        case '^':
        case '%':
          $result = rtrim($result) .' '. $text .' ';
          break;

        case '&':
          if (substr(rtrim($result), -1) == '=' || substr(rtrim($result), -1) == '(' || substr(rtrim($result), -1) == ',') {
            $result .= $text;
          }
          else {
            $result = rtrim($result) .' '. $text;
            // Ampersands used to declare reference return value for
            // functions should not have trailing space.
            if (!$in_function_declaration) {
              $result .= ' ';
            }
          }
          break;

        case '-':
          $result = rtrim($result);
          // Do not add a space before negative numbers or variables.
          $c = substr($result, -1);
          // Do not add a space between closing parenthesis and negative arithmetic operators.
          if ($c == '(') {
            $result .= ltrim($text);
          }
          // Add a space in front of the following chars, but not after them.
          elseif ($c == '>' || $c == '=' || $c == ',' || $c == ':' || $c == '?') {
            $result .= ' '. $text;
          }
          // Default arithmetic operator behavior.
          else {
            $result .= ' '. $text .' ';
          }
          break;

        case '"':
          // Toggle quote if the char is not escaped.
          if (rtrim($result) != "\\") {
            $in_quote = $in_quote ? false : true;
          }
          if (substr($result, -3) == ' . ') {
            // Write string concatenation character directly before strings.
            $result = rtrim($result);
          }
          $result .= $text;
          break;

        default:
          $result .= $text;
          break;
      }
    }
    else {
      // If we get here, then we have found not a single char, but a token.
      // See <http://www.php.net/manual/en/tokens.php> for a reference.

      // Fetch token array.
      list($id, $text) = $token;

      // Debugging:
      /*
      if ($lasttoken[0] == T_WHITESPACE) {
        $result .= token_name($id);
      }
      */

      switch ($id) {
        case T_ARRAY:
          // Write array in lowercase.
          $result .= strtolower(trim($text));
          // Mark the next parenthesis level (we haven't consumed that token
          // yet) as an array.
          $in_array[$parenthesis + 1] = TRUE;
          break;

        case T_OPEN_TAG:
        case T_OPEN_TAG_WITH_ECHO:
          $in_php = true;
          // Add a line break between two PHP tags.
          if (substr(rtrim($result), -2) == '?>') {
            coder_br($result);
          }
          $result .= trim($text);
          if ($first_php_tag) {
            coder_br($result);
            $first_php_tag = FALSE;
          }
          else {
            $result .= ' ';
          }
          break;

        case T_CLOSE_TAG:
          $in_php = false;
          // Remove preceding line break for inline PHP output in HTML.
          if (substr(rtrim($result), -1) == ';' || substr(rtrim($result), -1) == ':') {
            $result = rtrim($result) .' ';
          }
          $result .= trim($text);
          break;

        case T_OBJECT_OPERATOR:
          $in_object = true;
          $result .= trim($text);
          break;

        case T_CONSTANT_ENCAPSED_STRING:
          // Handle special string concatenation case: 'bar' . 'baz'
          $c = substr($result, -3);
          if ($c == '". ' || $c == '\'. ') {
            $result = rtrim($result, ' .');
            $result .= ' . ';
          }
          // Handle special string concatenation case: 'bar'. $foo
          elseif (substr($result, -2) == '. ') {
            $result = rtrim($result);
          }
          // Move on to T_STRING / T_VARIABLE.
        case T_STRING:
        case T_VARIABLE:
          // No space after object operator ($foo->bar) and error suppression (@function()).
          if ($in_object || $in_at) {
            $result    = rtrim($result) . trim($text);
            $in_object = false;
            $in_at     = false;
          }
          else {
            // Insert a space after right parenthesis, but not after type casts.
            if (!in_array($lasttoken[0], array(T_ARRAY_CAST, T_BOOL_CAST, T_DOUBLE_CAST, T_INT_CAST, T_OBJECT_CAST, T_STRING_CAST, T_UNSET_CAST))) {
              coder_add_space($result);
            }
            $result .= trim($text);
          }
          $in_variable = true;
          break;

        case T_ENCAPSED_AND_WHITESPACE:
          $result .= $text;
          break;

        case T_WHITESPACE:
          // Avoid duplicate line feeds outside arrays.
          $c = $parenthesis ? 0 : 1;

          for ($c, $cc = substr_count($text, "\n"); $c < $cc; ++$c) {
            // Newlines were added; not after semicolon anymore
            if ($parenthesis) {
              // Add extra indent for each parenthesis in multiline definitions (f.e. arrays).
              $_coder_indent = $_coder_indent + $parenthesis;
              $result = rtrim($result);
              coder_br($result);
              $_coder_indent = $_coder_indent - $parenthesis;
            }
            else {
              // Discard any whitespace, just insert a line break.
              coder_br($result);
            }
          }

          // If there were newlines present inside a parenthesis,
          // turn on multiline mode.
          if ($cc && $parenthesis) {
            $in_multiline[$parenthesis] = TRUE;
          }

          // If there were newlines present, move inline comments above.
          if ($cc) {
            $after_semicolon = FALSE;
            $after_case = FALSE;
          }
          
          $in_variable = FALSE;
          break;

        case T_IF:
        case T_FOR:
        case T_FOREACH:
        case T_SWITCH:
        case T_GLOBAL:
        case T_STATIC:
        case T_ECHO:
        case T_PRINT:
        case T_NEW:
        case T_REQUIRE:
        case T_REQUIRE_ONCE:
        case T_INCLUDE:
        case T_INCLUDE_ONCE:
        case T_VAR:
          coder_add_space($result);
          // Append a space.
          $result .= trim($text) .' ';
          break;

        case T_DO:
          $result .= trim($text);
          $in_do_while = true;
          break;

        case T_WHILE:
          if ($in_do_while && substr(rtrim($result), -1) === '}') {
            // Write while after right parenthesis for do {...} while().
            $result = rtrim($result) .' ';
            $in_do_while = false;
          }
          // Append a space.
          $result .= trim($text) .' ';
          break;

        case T_ELSE:
        case T_ELSEIF:
          // Write else and else if to a new line.
          $result = rtrim($result);
          coder_br($result);
          $result .= trim($text) .' ';
          break;

        case T_CASE:
        case T_DEFAULT:
          $braces_in_case = 0;
          $result         = rtrim($result);
          $after_case     = true;
          if (!$in_case) {
            $in_case = true;
            // Add a line break between cases.
            if (substr($result, -1) != '{') {
              coder_br($result);
            }
          }
          else {
            // Decrease current indent to align multiple cases.
            --$_coder_indent;
          }
          coder_br($result);
          $result .= trim($text) .' ';
          break;

        case T_BREAK:
          // Write break to a new line.
          $result = rtrim($result);
          coder_br($result);
          $result .= trim($text);
          if ($in_case && !$braces_in_case) {
            --$_coder_indent;
            $in_case = false;
          }
          break;

        case T_RETURN:
        case T_CONTINUE:
          coder_add_space($result);
          $result .= trim($text) .' ';
          // Decrease indent only if we're not in a control structure inside a case.
          if ($in_case && !$braces_in_case) {
            --$_coder_indent;
            $in_case = false;
          }
          break;

        case T_ABSTRACT:
        case T_PRIVATE:
        case T_PUBLIC:
        case T_PROTECTED:
          // Class member function properties must be treated similar to
          // T_FUNCTION, but without line-break after the token. Because more
          // than one of these tokens can appear in front of a function token,
          // we need another white-space control variable.
          coder_br($result);
          $result .= trim($text) .' ';
          break;
          
        case T_FUNCTION:
          $in_function_declaration = true;
          // Fall through.
        case T_CLASS:
          // Write function and class to new lines.
          $result = rtrim($result);
          if (substr($result, -1) == '}') {
            coder_br($result);
          }
          coder_br($result);
          $result .= trim($text) .' ';
          break;

        case T_EXTENDS:
        case T_INSTANCEOF:
          // Add space before and after 'extends' and 'instanceof'.
          $result = rtrim($result);
          $result .= ' '. trim($text) .' ';
          break;

        case T_AND_EQUAL:
        case T_AS:
        case T_BOOLEAN_AND:
        case T_BOOLEAN_OR:
        case T_CONCAT_EQUAL:
        case T_DIV_EQUAL:
        case T_DOUBLE_ARROW:
        case T_IS_EQUAL:
        case T_IS_NOT_EQUAL:
        case T_IS_IDENTICAL:
        case T_IS_NOT_IDENTICAL:
        case T_IS_GREATER_OR_EQUAL:
        case T_IS_SMALLER_OR_EQUAL:
        case T_LOGICAL_AND:
        case T_LOGICAL_OR:
        case T_LOGICAL_XOR:
        case T_MINUS_EQUAL:
        case T_MOD_EQUAL:
        case T_MUL_EQUAL:
        case T_OR_EQUAL:
        case T_PLUS_EQUAL:
        case T_SL:
        case T_SL_EQUAL:
        case T_SR:
        case T_SR_EQUAL:
        case T_XOR_EQUAL:
          // Surround operators with spaces.
          if (substr($result, -1) != ' ') {
            // $result must not be trimmed to allow multi-line if-clauses.
            $result .= ' ';
          }
          $result .= trim($text) .' ';
          break;

        case T_COMMENT:
        case T_ML_COMMENT:
        case T_DOC_COMMENT:
          if (substr($text, 0, 3) == '/**') {
            // Prepend a new line.
            $result = rtrim($result);
            coder_br($result);
            coder_br($result);

            // Remove carriage returns.
            $text = str_replace("\r", '', $text);

            $lines = explode("\n", $text);
            $params_fixed = false;
            for ($l = 0; $l < count($lines); ++$l) {
              $lines[$l] = trim($lines[$l]);

              // Add a new line between function description and first parameter description.
              if (!$params_fixed && substr($lines[$l], 0, 8) == '* @param' && $lines[$l - 1] != '*') {
                $result .= ' *';
                coder_br($result);
                $params_fixed = true;
              }
              else if (!$params_fixed && substr($lines[$l], 0, 8) == '* @param') {
                // Do nothing if parameter description is properly formatted.
                $params_fixed = true;
              }

              // Add a new line between function params and return.
              if (substr($lines[$l], 0, 9) == '* @return' && $lines[$l - 1] != '*') {
                $result .= ' *';
                coder_br($result);
              }

              // Add one space indent to get ' *[...]'.
              if ($l > 0) {
                $result .= ' ';
              }
              $result .= $lines[$l];
              if ($l < count($lines)) {
                coder_br($result);
              }
            }
          }
          else {
            // Move the comment above if it's embedded.
            $statement = false;
            if ($after_semicolon && !$after_case) {
              $nl_position     = strrpos(rtrim($result, " \n"), "\n");
              $statement       = substr($result, $nl_position);
              $result          = substr($result, 0, $nl_position);
              $after_semicolon = false;
              coder_br($result);
            }
            $result .= trim($text);
            if ($parenthesis) {
              // Add extra indent for each parenthesis in multiline definitions (f.e. arrays).
              $_coder_indent = $_coder_indent + $parenthesis;
              $result = rtrim($result);
              coder_br($result);
              $_coder_indent = $_coder_indent - $parenthesis;
            }
            else {
              // Discard any whitespace, just insert a line break.
              coder_br($result);
            }
            if ($statement) {
              $result = rtrim($result, "\n ");
              $result .= $statement;
            }
          }
          break;

        case T_INLINE_HTML:
          $result .= $text;
          break;

        case T_START_HEREDOC:
          $result .= trim($text);
          coder_br($result, false);
          $in_heredoc = true;
          break;

        case T_END_HEREDOC:
          $result .= trim($text);
          coder_br($result, false);
          $in_heredoc = false;
          break;

        default:
          $result .= trim($text);
          break;
      }

      // Store last token.
      $lasttoken = $token;
    }
  }
  return $result;
}

/**
 * Generate a line feed including current line indent.
 *
 * This function will also remove all line indentation from the
 * previous line if no text was added.
 *
 * @param &$result
 *   Result variable to append break and indent to, passed by reference.
 * @param $add_indent
 *   Whether to add current line indent after line feed.
 */
function coder_br(&$result, $add_indent = true) {
  global $_coder_indent;

  // Scan result backwards for whitespace.
  for ($i = strlen($result) - 1; $i >= 0; $i--) {
    if ($result[$i] == ' ') {
      continue;
    }
    if ($result[$i] == "\n") {
      $result = rtrim($result, ' ');
      break;
    }
    // Non-whitespace was encountered, no changes necessary.
    break;
  }

  $output = "\n";
  if ($add_indent && $_coder_indent >= 0) {
    $output .= str_repeat('  ', $_coder_indent);
  }
  $result .= $output;
}

/**
 * Write a space in certain conditions.
 *
 * A conditional space is needed after a right parenthesis of an if statement
 * that is not followed by curly braces.
 *
 * @param $result
 *   Current result string that will be checked.
 *
 * @return
 *   Resulting string with or without an additional space.
 */
function coder_add_space(&$result) {
  if (substr($result, -1) == ')') {
    $result .= ' ';
  }
}

/**
 * Trim overall code.
 *
 * Strips whitespace at the beginning and end of code,
 * removes the closing PHP tag and appends two empty lines.
 */
function coder_trim_php($code) {
  // Remove surrounding whitespace.
  $code = trim($code);

  // Insert CVS keyword Id.
  // Search in the very first 1000 chars, insert only one instance.
  if (strpos(substr($code, 0, 1000), '$Id') === false) {
    $code = preg_replace('/<\?php\n/', "<?php\n// \$Id\$\n\n", $code, 1);
  }

  // Remove closing PHP tag.
  if (substr($code, -2) == '?>') {
    $code = rtrim($code, '?>');
  }

  // Append two empty lines.
  $code .= str_repeat(chr(10), 2);

  return $code;
}

/**
 * Execute special tasks on source code.
 *
 * This function works similar to the Drupal hook and forms system. It searches
 * for all defined functions with the given prefix and performs a preg_replace
 * on the source code for each of these functions.
 *
 * Processor functions are defined with a associative array containing the
 * following keys with the corresponding values:
 *   #title
 *      A human readable text describing what the processor actually does.
 *   #search
 *      The regular expression to search for.
 *   #replace
 *      The replacement text for each match.
 *
 * Optional definitions:
 *   #debug
 *      Set this to true to directly output the results of preg_match_all and
 *      exit script execution after this processor.
 *
 * @param string $code
 *      The source code to process.
 * @param string $prefix
 *      Prefix of the functions to execute.
 *
 * @return
 *      The processed source code.
 */
function coder_exec_processors($code, $prefix = '') {
  if (empty($prefix)) {
    return;
  }
  $tasks = get_defined_functions();
  $tasks = $tasks['user'];
  for ($c = 0, $cc = count($tasks); $c < $cc; ++$c) {
    if (strpos($tasks[$c], $prefix) === false) {
      unset($tasks[$c]);
    }
    else {
      $tasks[$tasks[$c]] = call_user_func($tasks[$c]);
      unset($tasks[$c]);
    }
  }
  uasort($tasks, 'coder_order_processors');
  foreach ($tasks as $func => $task) {
    if (!isset($task['#search']) || (!isset($task['#replace']) && !isset($task['#replace_callback']))) {
      continue;
    }
    if (isset($task['#debug'])) {
      // Output regular expression results if debugging is enabled.
      preg_match_all($task['#search'], $code, $matches, PREG_SET_ORDER);
      echo "<pre>";
      var_dump($matches);
      echo "</pre>\n";
      // Exit immediately in debugging mode.
      exit;
    }
    if (isset($task['#replace_callback'])) {
      $code = preg_replace_callback($task['#search'], $task['#replace_callback'], $code);
    }
    else {
      $code = preg_replace($task['#search'], $task['#replace'], $code);
    }
  }

  return $code;
}

/**
 * Orders preprocessors by weight.
 *
 * @see coder_exec_processors()
 */
function coder_order_processors($a, $b) {
  if (isset($a['#weight']) && isset($b['#weight'])) {
    return $a['#weight'] - $b['#weight'];
  }
  else {
    return isset($a['#weight']) ? false : true;
  }
}

/**
 * @defgroup coder_preprocessor Preprocessors.
 * @{
 */
function coder_preprocessor_line_breaks_win() {
  return array(
    '#title' => 'Convert Windows line breaks to Unix format.',
    '#weight' => 1,
    '#search' => "/\r\n/",
    '#replace' => "\n",
  );
}

function coder_preprocessor_line_breaks_mac() {
  return array(
    '#title' => 'Convert Macintosh line breaks to Unix format.',
    '#weight' => 2,
    '#search' => "/\r/",
    '#replace' => "\n",
  );
}

function coder_preprocessor_php() {
  return array(
    '#title' => 'Always use &lt;?php ?&gt; to delimit PHP code, not the &lt;? ?&gt; shorthands.',
    '#search' => '/<\?(\s)/',
    '#replace' => "<?php$1",
  );
}

function coder_preprocessor_switch_duplicate_exit() {
  return array(
    '#title' => 'Either exit a switch case with return *or* break.',
    '#search' => '/(return\s+.+;)\s+break;/',
    //'#replace' => '$1',
  );
}

/**
 * @} End of "defgroup coder_preprocessor".
 */

/**
 * @defgroup coder_postprocessor Postprocessors.
 * @{
 */
function coder_postprocessor_cvs_id() {
  return array(
    '#title' => 'If the CVS keyword Id already exists, append a new line after it.',
    '#search' => '@^(//.*\$Id.*\$)$@m',
    '#replace' => "$1\n",
  );
}

function coder_postprocessor_multiple_vars() {
  // @todo Prevent matching of multiple lines separated by a blank line 26/03/2007 sun.
  return array(
    '#title' => 'Align equal signs of multiple variable assignments in the same column.',
    // \n? * matches whitespace, but only one new line.
    // \$.+? matches variable names.
    // {3,} requires the pattern to match at least 3 times.
    '#search' => '/^(\n? *\$.+? = .+?$){3,}/m',
    '#replace_callback' => 'coder_replace_multiple_vars',
  );
}

function coder_replace_multiple_vars($matches) {
  // Retrieve all variable name = variable value pairs.
  preg_match_all('/^(\s*)(\$.+?) (.?)= (.+?$)/m', $matches[0], $vars, PREG_SET_ORDER);

  // Determine the longest variable name.
  $maxlength = 0;
  foreach ($vars as $var) {
    if (strlen($var[2]) > $maxlength) {
      $maxlength = strlen($var[2] . $var[3]);
    }
  }

  // Realign variable values at the longest variable names.
  $return = '';
  $extra_spaces = 0;
  for ($c = 0, $cc = count($vars); $c < $cc; ++$c) {
    if ($maxlength <= 20) {
      $extra_spaces = $maxlength - strlen($vars[$c][2] . $vars[$c][3]);
    }
    $return .= $vars[$c][1] . $vars[$c][2];
    $return .= str_repeat(' ', $extra_spaces) .' '. $vars[$c][3] .'= ';
    $return .= $vars[$c][4];
    if ($c < $cc - 1) {
      // Append a line break, but not to the last variable assignment.
      $return .= "\n";
    }
  }

  return $return;
}

function coder_postprocessor_indent_multiline_array() {
  // Still buggy, disabled for now.
  return array(
    '#title' => 'Align equal signs of multiline array assignments in the same column.',
    // ?: prevents capturing
    // \s* initial whitespace
    // ([\'"]).+?\1 matches a string key
    // .+? matches any other key w/o whitespace
    // \s*=>\s* matches associative array arrow syntax
    // .+? matches value
    '#search' => '/^(?:\s*(?:(?:([\'"]).+?\1|.+?)\s*=>\s*.+?|\),\s?)$){3,}/mi',
    //'#replace_callback' => 'coder_replace_indent_multiline_array',
  );
}

function coder_replace_indent_multiline_array($matches) {
  // Separate out important components of the multiline array:
  // (\s*) matches existing indent as \1
  // (([\'"]).+?\2|\$.+?|[+\-]?(?:0x)?[0-9A-F]+) matches key as \2
  //    ([\'"]).+?\3 matches a quoted key, quote used is \3
  //    \.+? matches anything else
  // \),\s*? matches a closing parenthesis in a nested array
  // \s*=>\s* matches existing indentation and arrow to be discarded
  // (.+?) matches value as \4
  // {3,} requires three or more of these lines
  // mi enables multiline and caseless mode
  preg_match_all('/^(\s*)(?:(([\'"]).+?\3|\.+?)\s*=>\s*(.+?),?|\),)\s*?$/mi', $matches[0], $vars, PREG_SET_ORDER);
  // Determine max key length for varying indentations.
  $maxlengths = array();
  foreach ($vars as $var) {
    list(, $indent, $key) = $var;
    if (!isset($maxlengths[$indent])) {
      $maxlengths[$indent] = 0;
    }
    if (($t = strlen($key)) > $maxlengths[$indent]) {
      $maxlengths[$indent] = $t;
    }
  }
  // Reconstruct variable array declaration.
  $return = '';
  foreach ($vars as $var) {
    list(, $indent, $key, , $value) = $var;
    if ($key === null) {
      $return .= "$indent),\n";
      continue;
    }
    $spaces = str_repeat(' ', $maxlengths[$indent] - strlen($key));
    if ($value !== 'array(') {
      $comma = ',';
    }
    else {
      $comma = '';
    }
    $return .= "$indent$key$spaces => $value$comma\n";
  }
  $return = rtrim($return, "\n");
  return $return;
}

function coder_postprocessor_array_rearrange() {
  // @bug common.inc, comment.module:
  // Not yet working properly 25/03/2007 sun.
  return array(
    '#title' => 'Break array elements into separate lines, indented one level.',
    // ([\040\t]*) matches blanks and tabs.
    // (.*?array\() matches anything and 'array('.
    // ((.+ => .+, ){3,}) matches all array items, except the last one.
    // (.+ => ([^\(\)]+)) matches the last array item, excluding.
    //   arrays or functions (starting with a left parenthesis) (not supported yet).
    //'#search' => '/^([\040\t]*)(.*?array\()((.+ => .+, ){3,})(.+ => ([^\(\)]+))\)/m',
    '#replace_callback' => 'coder_replace_array_rearrange',
  );
}

function coder_replace_array_rearrange($matches) {
  // Retrieve all array items, except the last one.
  preg_match_all('/(.+? => .+?,) /', $matches[3], $items);

  // The original line including array(.
  $return = $matches[1] . $matches[2] ."\n";
  foreach ($items[1] as $item) {
    // All array items, except the last one, with extra indent.
    $return .= $matches[1] .'  '. $item ."\n";
  }
  // Last array item, with extra indent and comma.
  $return .= $matches[1] .'  '. $matches[5] .",\n";
  // Closing parenthesis (on a new line).
  $return .= $matches[1] .')';

  return $return;
}

function coder_postprocessor_if_curly_braces() {
  // @todo This is just a starting point for manual replacement at the moment.
  return array(
    '#title' => 'Use curly braces even in situations where they are technically optional.',
    '#search' => '/if \(.+\) [^\{]+;/',
    //'#replace_callback' => 'coder_replace_if_curly_braces',
  );
}

/**
 * @} End of "defgroup coder_postprocessor".
 */

/**
 * @defgroup coder_format_stub_functions
 * @{
 */
if (!function_exists('variable_get')) {
  function variable_get($name, $default) {
    global $conf;

    return isset($conf[$name]) ? $conf[$name] : $default;
  }
}
if (!function_exists('variable_set')) {
  function variable_set($name, $value) {
    global $conf;

    $conf[$name] = $value;
  }
}
if (!function_exists('t')) {
  function t($string, $args = 0) {
    if (!$args) {
      return $string;
    }
    else {
      return strtr($string, $args);
    }
  }
}
if (!function_exists('drupal_set_message')) {
  function drupal_set_message($message = NULL, $type = 'status') {
    echo $message;
  }
}
if (!function_exists('watchdog')) {
  function watchdog($type, $message, $severity = 0, $link = null) {
    return;
  }
}
if (!function_exists('check_plain')) {
  function check_plain($text) {
    return htmlspecialchars($text, ENT_QUOTES);
  }
}
if (!function_exists('conf_path')) {
  function conf_path() {
    return "sites/default";
  }
}

/**
 * @} End of "defgroup coder_format_stub_functions".
 */

