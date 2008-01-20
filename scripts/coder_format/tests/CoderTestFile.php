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
  var $input = array();

  /* Expected output */
  var $expect = array();

  /* Actual result */
  var $actual = array();

  /* Whether or not <?php and other stuff should be added */
  var $full = array();

  /**
   * Loads this class from a file.
   *
   * @param string $filename
   *   A filename to load.
   */
  function load($filename) {
    $this->filename = $filename;
    $fh             = fopen($filename, 'r');
    $state          = '';
    $php_stripped   = FALSE;
    $line_no        = 0;
    $unit           = 0;
    
    while (($line = fgets($fh)) !== false) {
      // Normalize newlines.
      $line = rtrim($line, "\n\r");
      // Strip first PHP tag, if existent.
      if (!$php_stripped && !$line_no && strpos($line, '<?php') === 0) {
        $php_stripped = TRUE;
        continue;
      }
      // Detect INPUT and EXPECT sections.
      if (substr($line, 0, 2) == '--') {
        $state = trim($line, ' -');
        
        // If a new INPUT section begins, start a new unit.
        if ($state == 'INPUT') {
          $unit++;
        }
        continue;
      }
      if (!$state) {
        list($key, $line) = explode(': ', $line, 2);
      }
      else {
        $key = $state;
      }
      switch ($key) {
        case 'TEST':
          $this->test = $line;
          break;

        case 'FULL':
          $this->full[$unit] = (bool)$line;
          break;

        case 'INPUT':
          $this->input[$unit] .= $line ."\n";
          break;

        case 'EXPECT':
          $this->expect[$unit] .= $line ."\n";
          break;
      }
    }
    fclose($fh);
    foreach (range(1, $unit) as $unit) {
      // If no EXPECTed code was defined, INPUT shouldn't be altered.
      if (!isset($this->expect[$unit])) {
        $this->expect[$unit] = $this->input[$unit];
      }
      // If FULL was not defined, add a PHP header to contents.
      if (!$this->full[$unit]) {
        $prepend             = "<?php\n// $". "Id$\n\n";
        $this->input[$unit]  = $prepend . rtrim($this->input[$unit], "\n") ."\n\n";
        $this->expect[$unit] = $prepend . rtrim($this->expect[$unit], "\n") ."\n\n";
      }
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
    
    // Perform test.
    // Test passes until proven invalid.
    $valid = TRUE;
    foreach ($this->input as $unit => $content) {
      // Parse input and store results.
      $this->actual[$unit] = coder_format_string_all($this->input[$unit]);
      
      // Let this test fail, if a unit fails.
      if ($this->expect[$unit] !== $this->actual[$unit]) {
        $valid = FALSE;
      }
    }
    
    return $valid;
  }

  /**
   * Implements SimpleExpectation::testMessage().
   */
  function testMessage() {
    $message = $this->test .' test in '. htmlspecialchars(basename($this->filename));
    return $message;
  }

  /**
   * Renders the test with an HTML diff table.
   */
  function render() {
    drupal_add_css(drupal_get_path('module', 'coder') .'/scripts/coder_format/tests/coder-diff.css', 'module', 'all', false);
    
    foreach ($this->input as $unit => $content) {
      // Do not output passed units.
      if ($this->expect[$unit] === $this->actual[$unit]) {
        continue;
      }
      
      $diff     = new Text_Diff('auto', array(explode("\n", $this->expect[$unit]), explode("\n", $this->actual[$unit])));
      $renderer = new Text_Diff_Renderer_parallel($this->test .' test in '. htmlspecialchars(basename($this->filename)));
      
      $message .= $renderer->render($diff);
    }
    
    return $message;
  }
}

/**
 * Parallel diff renderer for HTML tables with original text on left,
 * new text on right, and changed text highlighted with appropriate classes.
 */
class Text_Diff_Renderer_parallel extends Text_Diff_Renderer {
  /* String header for left column */
  var $original = 'Expected';

  /* String header for right column */
  var $final = 'Actual';
  
  // These are big to ensure entire string is output.
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

