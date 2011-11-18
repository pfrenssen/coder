<?php

#Discouraged comment style

//Inline commands ned to be prefixed by a space after the //

// PHP Constants should be written in CAPITAL lettres
true;
false;
null;

// Missing spaces
if(TRUE){
  TRUE;
}
if (TRUE){
  TRUE;
}
if(TRUE) {
  TRUE;
}

define('C_TEST',1);

// Has whitespace at the end of the line.
$whitespaces = 'Yes, Please';  

// Operators - must have a space before and after
$i= 0;
$i+= 0;
$i-= 0;
$i== 0;
$i=== 0;
$i!== 0;
$i!= 0;
$i> 0;
$i< 0;
$i>= 0;
$i<= 0;
$i=0;
$i+=0;
$i-=0;
$i==0;
$i===0;
$i!==0;
$i!=0;
$i>0;
$i<0;
$i>=0;
$i<=0;
$i =0;
$i +=0;
$i -=0;
$i ==0;
$i  ==0;
$i !=0;
$i >0;
$i <0;
$i >=0;
$i <=0;
$i  += 0;
$i  -= 0;
$i  == 0;
$i  === 0;
$i  !== 0;
$i  != 0;
$i  > 0;
$i  < 0;
$i  >= 0;
$i  <= 0;
$i =  0;
$i +=  0;
$i -=  0;
$i ==  0;
$i ===  0;
$i !==  0;
$i !=  0;
$i >  0;
$i <  0;
$i >=  0;
$i <=  0;
// Arihtmetic operators, too.
$i = 1+1;
$i = 1- 1;
$i = 1 /2;
$i = 1  * 1;


// Unary operators must not have a space
$i --;
-- $i;
$i ++;
++ $i;
$i = - 1;
$i = + 1;
$i =  -1;
$i =  +1;
array('i' => - 1);
array('i' => + 1);
$i = (1 == - 1);
$i = (1 === - 1);
$i = (1 == + 1);
$i = (1 === + 1);
range(-50, - 45);
range(- 50, -45);


// Casting must have one space
(int)$i;
(int)  $i;


// Last item of a multiline array has to be followed by a
// comma. On inline arrays not!
$a = array('1', '2', '3',);
$a = array('1', '2', '3');
$a = array(
  '1',
  '2',
  '3'
);
$a = array('1', '2', array('3', ));
$a = array('1', '2', array('3',), );
$a = array('1', '2', array('3'), );
$a = array('1', '2', array('3',), );
$a = array('1', '2', array(
  '3'),
);
$a = array(
  '1',
  '2',
  array('3',)
);

// Item assignment operators must be prefixed and followed by a space
$a = array('one'=>'1');
$a = array('one'=> '1');
$a = array('one' =>'1');
foreach ( $a as $key=>$value) {
}
foreach ( $a as $key =>$value) {
}
foreach ( $a as $key=> $value) {
}



// elseif and else must have a newline after the curly braces of the former condition
if(TRUE || TRUE){
  $i;
}elseif(TRUE && TRUE){
  $i;
}else{
  $i;
}

// elseif and else must have a newline after the curly braces of the former condition
if (TRUE || TRUE) {
  $i;
}
elseif(TRUE && TRUE){
  $i;
}
else{
  $i;
}

// If conditions have a space before and after the condition parenthesis
if(TRUE || TRUE) {
  $i;
}elseif(TRUE && TRUE) {
  $i;
}else {
  $i;
}

// else if is not allowed
if (TRUE) {
  $i;
}else if (TRUE) {
  $i;
}
// Break has to be intended 2 spaces
switch ($condition) {
  case 1:
    $i;
  break;

  case 2:
    $i;
      break;
}

// Missing Spaces
switch($condition) {
  default:
    $i;
}
switch($condition){
  default:
    $i;
}
switch ($condition){
  default:
    $i;
}

// Missing space after "do"
do{
  $i;
} while ($condition);
// Missing space before "while"
do {
  $i;
}while ($condition);
// Missing space after "while"
do {
  $i;
} while($condition);

/**
 * Short description
 *
 * We use doxygen style comments.
 * What's sad because eclipse PDT and
 * PEAR CodeSniffer base on phpDoc comment style.
 * Makes working with drupal not easier :|
 *
 * @param $field1
 *  Doxygen style comments
 * @param $field2
 *  Doxygen style comments
 * @param $field3
 *  Doxygen style comments
 * @return
 *  Doxygen style comments
 */
function foo($field1, $field2, $field3 = NULL) {
  $system["description"] = t("This module inserts funny text into posts randomly.");
  return $system[$field];
}

/**
 * use a single space between the closing parenthesis and the open bracket
 */
function foo()  {

}

/**
 * use a single space between the closing parenthesis and the open bracket
 */
function foo(){

}

// There has to be a space betrween the comma and the next argument
$var = foo($i,$i);

// Multiline function call - all the parameters have to be on the next line
// I don't thing that this rule is strictly followed...
$var = foo($i,
  $i,
  $i
);

// Multiline function call - closing parenthesis has to be on a own line
$var = foo(
  $i,
  $i,
  $i);


// Only multiline comments with /** should be used to comment classes
// Curly brace has to be on the same line
class Bar
{
  // Private properties must not have a prefix
  private $_secret = 1;

  // Public properties must not have a prefix
  protected $_foo = 1;

  // Public properties must not have a prefix
  public $_bar = 1;

  // Public static variables use camelCase.
  public static $base_path = NULL;
}

// Comments must have content ;) and there must be a space after the class name
/**
 *
 */
class FooBar{
}

// When calling class constructors with no arguments, always include parentheses:
$bar = new Bar;
// Check for spaces between classname and opening parenthesis
$bar = new Bar ();
// Check argument formating
$bar = new Bar($i,$i);

// Concatenation - there has to be a space
$i ."test";
$i .'test';
$i .$i;
$i .C_TEST;
$i."test";
$i.'test';
$i.$i;
$i.C_TEST;
$i. "test";
$i. 'test';
$i. $i;
$i. C_TEST;

/**
 *
 */
class FunctionTest {
  public function _foo() {

  }

  protected function _bar() {

  }

  private function foobar() {

  }
}

function _refix() {

}

// Usage of t() - there should be no escaping.
t('She\'s a good person.');
t("This is a \"fancy\" string.");
