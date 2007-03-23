<?php
// $Id$

/** @file
 * This include file implements coder functionality for Performance
 *
 * Todo: The rules for this review are not yet complete.
 */

function coder_performance_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => 'TRUE|FALSE|NULL',
      '#case-sensitive' => true,
      '#warning_callback' => '_coder_performance_case_variabletype_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(?!\'|\*).*"[^\\\n]+".*(?!\')',
      '#not' => '\'.+\'',
      '#source' => 'all',
      '#warning_callback' => '_coder_performance_double_quotes_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\$\w+\+\+',
      '#warning_callback' => '_coder_performance_increment_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'for\s*\([^,]+;.+count',
      '#warning_callback' => '_coder_performance_for_count_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\(strlen\(\$\w+\) <=? \d+\)',
      '#warning_callback' => '_coder_performance_string_length_comparison_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[^=] print',
      '#warning_callback' => '_coder_performance_print_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'in_array\(\'\', \$\w+\)',
      '#warning_callback' => '_coder_performance_in_array_warning',
    ),

    /* ctype not available on many systems - http://drupal.org/node/72865
    array(
      '#type' => 'regex',
      '#value' => 'preg_match\([\'"].\[(.-.)+\].?.[\'"]',
      '#source' => 'all',
      '#warning_callback' => '_coder_performance_ctype_warning',
    ), */
  );
  $review = array(
    '#title' => 'Performance Optimization',
    '#link' => 'http://drupal.org/node/110128',
    '#rules' => $rules,
    '#severity' => 'minor',
  );
  return array('performance' => $review);
}

function _coder_performance_case_variabletype_warning() {
  return array(
    '#warning' => t('<code class="good">true</code> is faster than <code class="bad">TRUE</code>, also applies to <code class="bad">FALSE</code> and <code class="bad">NULL</code>'),
    '#description' => t('This is because when looking for constants PHP does a hash lookup for the name as is. And since names are always stored lowercased, by using them you avoid 2 hash lookups.'),
  );
}

function _coder_performance_double_quotes_warning() {
  return array(
    '#warning' => t('<code class="good">\'foo\'</code> is faster than <code class="bad">"foo"</code>'),
    '#description' => t('This is because a double quotes encapsulated string is parsed for variables and escaped characters. Always use single quotes, if not necessarily needed.'),
  );
}

function _coder_performance_increment_warning() {
  return array(
    '#warning' => t('<code class="good">++$i</code> is faster than <code class="bad">$i++</code>'),
    '#description' => t('When incrementing or decrementing the value of the variable <code>$i++</code> happens to be a tad slower than <code>++$i</code>. This is something PHP specific and does not apply to other languages, so don\'t go modifying your C or Java code thinking it\'ll suddenly become faster, it won\'t. <code>++$i</code> happens to be faster in PHP because instead of 4 opcodes used for <code>$i++</code> you only need 3. Post incrementation actually causes in the creation of a temporary var that is then incremented. While pre-incrementation increases the original value directly. This is one of the optimization that opcode optimized like Zend\'s PHP optimizer. It is a still a good idea to keep in mind since not all opcode optimizers perform this optimization and there are plenty of ISPs and servers running without an opcode optimizer.'),
  );
}

function _coder_performance_for_count_warning() {
  return array(
    '#warning' => t('<code class="good">for ($c = 0, $cc = count($foo); $c < $cc; ++$c)</code> is faster than <code class="bad">for ($c = 0; $c < count($foo); ++$c)</code>'),
    '#description' => t('In PHP a <code>for</code> loop with a <code>count()</code> inside the control block is executed on <em>every</em> loop iteration.'),
  );
}

function _coder_performance_string_length_comparison_warning() {
  return array(
    '#warning' => t('<code class="good">if (!isset($foo{6}))</code> is faster than <code class="bad">if (strlen($foo) < 5)</code> or <code class="bad">if (strlen($foo) <= 6)</code>'),
    '#description' => t('When working with strings and you need to check that the string is either of a certain length you\'d understandably would want to use the <code>strlen()</code> function. This function is pretty quick since it\'s operation does not perform any calculation but merely return the already known length of a string available in the zval structure (internal C struct used to store variables in PHP). However because <code>strlen()</code> is a function it is still somewhat slow because the function call requires several operations such as lowercase &amp; hashtable lookup followed by the execution of said function. Calling <code>isset()</code> happens to be faster than <code>strlen()</code> because unlike <code>strlen()</code>, <code>isset()</code> is a language construct and not a function meaning that it\'s execution does not require function lookups and lowercase. This means you have virtually no overhead on top of the actual code that determines the string\'s length.'),
  );
}

function _coder_performance_print_warning() {
  return array(
    '#warning' => t('<code class="good">echo</code> is faster than <code class="bad">print</code> (if return value from <code class="bad">print</code> is not used)'),
    '#description' => t('Even both of these output mechanism are language constructs, if you benchmark the two you will quickly discover that <code>print()</code> is slower than <code>echo()</code>. The reason for that is quite simple, <code>print</code> function will return a status indicating if it was successful or not, while <code>echo</code> simply prints the text and nothing more. Since in most cases (haven\'t seen one yet) this status is not necessary and is almost never used it is pointless and simply adds unnecessary overhead.'),
  );
}

function _coder_performance_in_array_warning() {
  return array(
    '#warning' => t('<code class="good">if (isset($array[\'foo\']))</code> is faster than <code class="bad">if (in_array(\'foo\', $array))</code>'),
    '#description' => t('Another common operation in PHP scripts is array searching. This process can be quite slow as regular search mechanism such as <code>in_array()</code> or manual implementation work by iterating through the entire array. This can be quite a performance hit if you are searching through a large array or need to perform the searches frequently. So what can you do? Well, you can do a trick that relies upon the way that Zend Engine stores array data. Internally arrays are stored inside hash tables when their array element (key) is the key of the hashtables used to find the data and result is the value associated with that key. Since hashtable lookups are quite fast, you can simplify array searching by making the data you intend to search through the key of the array, then searching for the data is as simple as <code>isset($foo[$bar]))</code>. This search mechanism is way faster than manual array iteration, even though having string keys maybe more memory intensive than using simple numeric keys.<br /><br />
        Example:<br />
        <code>$keys = array("apples", "oranges", "mangoes", "tomatoes", "pickles");<br />
        if (in_array(\'mangoes\', $keys)) { ... }<br /></code>
        <br />
        vs.<br />
        <br />
        <code>$keys = array("apples" => 1, "oranges" => 1, "mangoes" => 1, "tomatoes" => 1, "pickles" => 1);<br />
        if (isset($keys[\'mangoes\'])) { ... }<br /></code>
        <br />
        The bottom search mechanism is roughly 3 times faster.'),
  );
}

/* ctype not available on many systems - http://drupal.org/node/72865
function _coder_performance_ctype_warning() {
  return array(
    '#warning' => t('<code class="good">ctype_digit($foo);</code> and <code class="good">ctype_alpha($foo);</code> are faster than <code class="bad">preg_match(\'/[0-9]+/\', $foo);</code> or <code class="bad">preg_match(\'/[a-z]+/\', $foo);</code>'),
    '#description' => t('Many scripts tend to reply on regular expression to validate the input specified by user. While validating input is a superb idea, doing so via regular expression can be quite slow. In many cases the process of validation merely involved checking the source string against a certain character list such as A-Z or 0-9, etc. Instead of using regex in many instances you can instead use the ctype extension (enabled by default since PHP 4.2.0) to do the same. The ctype extension offers a series of function wrappers around C\'s is*() function that check whether a particular character is within a certain range. Unlike the C function that can only work on one character at a time, PHP\'s can operate on entire strings and is far faster than equivalent regular expressions.'),
  );
} */
