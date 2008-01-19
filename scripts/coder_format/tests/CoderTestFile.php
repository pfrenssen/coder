<?php
// $Id$


/**
 * Represents coder test file for full coder_format_string_all() tests.
 */
class CoderTestFile extends SimpleExpectation {
  /* Filename of test */
  var $filename;

  /* Test name */
  var $test;

  /* PHP to be parsed */
  var $input = '';

  /* Expected output */
  var $expect = '';

  /* Actual result */
  var $actual;

  /* Whether or not <?php and other stuff should be added */
  var $full;

  /**
   * Loads this class from a file.
   *
   * @param $filename String filename to load
   */
  function load($filename) {
    $this->filename = $filename;
    $fh             = fopen($filename, 'r');
    $state          = '';
    $php_stripped   = FALSE;
    
    while (($line = fgets($fh)) !== false) {
      // Normalize newlines.
      $line = rtrim($line, "\n\r");
      // Strip first PHP tag, if existent.
      if (!$php_stripped && strpos($line, '<?php') === 0) {
        $php_stripped = TRUE;
        continue;
      }
      if (substr($line, 0, 2) == '--') {
        // Detected section.
        $state = trim($line, ' -');
        continue;
      }
      if (!$state) {
        list($key, $line) = explode(': ', $line, 2);
      }
      else {
        $key = $state;
      }
      switch ($key) {
        case 'INPUT':
          $this->input .= $line ."\n";
          break;

        case 'EXPECT':
          $this->expect .= $line ."\n";
          break;

        case 'TEST':
          $this->test = $line;
          break;

        case 'FULL':
          $this->full = (bool)$line;
          break;
      }
    }
    fclose($fh);
    if ($this->expect === '') {
      $this->expect = $this->input;
    }
    if (!$this->full) {
      $prepend      = "<?php\n//$". "Id$\n\n";
      $this->input  = $prepend . trim($this->input, "\n") ."\n\n";
      $this->expect = $prepend . trim($this->expect, "\n") ."\n\n";
    }
  }

  /**
   * Implements SimpleExpectation::test().
   *
   * @param $filename Filename of test file to test.
   */
  function test($filename = false) {
    if ($filename) {
      $this->load($filename);
    }
    $this->actual = coder_format_string_all($this->input);
    
    return $this->expect === $this->actual;
  }

  /**
   * Implements SimpleExpectation::testMessage().
   */
  function testMessage() {
    $message = $this->test .' test at '. htmlspecialchars($this->filename);
    return $message;
  }

  /**
   * Renders the test with an HTML diff table.
   */
  function render() {
    drupal_add_css(drupal_get_path('module', 'coder') .'/scripts/coder_format/tests/coder-diff.css', 'module', 'all', false);
    
    $diff     = new Text_Diff('auto', array(explode("\n", $this->expect), explode("\n", $this->actual)));
    $renderer = new Text_Diff_Renderer_parallel($this->test .' test at '. htmlspecialchars($this->filename));
    
    $renderer->original = 'Expected';
    $renderer->final    = 'Actual';
    
    $message .= $renderer->render($diff);
    return $message;
  }
}

/**
 * Parallel diff renderer for HTML tables with original text on left,
 * new text on right, and changed text highlighted with appropriate classes.
 */
class Text_Diff_Renderer_parallel extends Text_Diff_Renderer {
  /* String header for left column */
  var $original = 'Original';

  /* String header for right column */
  var $final = 'Final';
  // these are big to ensure entire string is output
  var $_leading_context_lines  = 10000;
  var $_trailing_context_lines = 10000;
  var $title;
  
  function Text_Diff_Renderer_parallel($title) {
    $this->title = $title;
  }

  function _blockHeader() {}

  function _startDiff() {
    return '<table class="diff"><thead><tr><th colspan="2">'. $this->title .'</th></tr><tr><th>'. $this->original .'</th><th>'. $this->final .'</th></tr></thead><tbody>';
  }

  function _endDiff() {
    return '</tbody></table>';
  }

  function _context($lines) {
    return '<tr><td><pre>'. htmlspecialchars(implode("\n", $lines)) .'</pre></td>
          <td><pre>'. htmlspecialchars(implode("\n", $lines)) .'</pre></td></tr>';
  }

  function _added($lines) {
    return '<tr><td>&nbsp;</td><td class="added"><pre>'. htmlspecialchars(implode("\n", $lines)) .'</pre></td></tr>';
  }

  function _deleted($lines) {
    return '<tr><td class="deleted"><pre>'. htmlspecialchars(implode("\n", $lines)) .'</pre></td><td>&nbsp;</td></tr>';
  }

  function _changed($orig, $final) {
    return '<tr class="changed"><td><pre>'. htmlspecialchars(implode("\n", $orig)) .'</pre></td>
        <td><pre>'. htmlspecialchars(implode("\n", $final)) .'</pre></td></tr>';
  }
}

