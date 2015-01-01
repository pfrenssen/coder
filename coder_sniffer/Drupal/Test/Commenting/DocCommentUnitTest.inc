<?php

/**
 * This doc comment ends in a dot, but has some white space after it. 
 */

/**
 * Separate different-type sections by a blank line.
 *
 * @param string $param
 *   Something incredibly useful.
 * @return bool
 *   Returns FALSE.
 * @throws Exception
 *   Thrown when $param is TRUE.
 * @ingroup sniffer
 * @deprecated
 * @see some_function()
 * @todo Fix coding standards.
 */
function test7($param) {
  if ($param) {
    throw new Exception();
  }
  return FALSE;
}
