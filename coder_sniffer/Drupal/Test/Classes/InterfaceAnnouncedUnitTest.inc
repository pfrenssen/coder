<?php

namespace Foo\Bar;

/**
 *
 */
interface FooInterface {

}

/**
 *
 */
interface BarInterface {

}

/**
 *
 */
interface BazInterface {

}

/**
 *
 */
class Foo implements FooInterface {

}

/**
 *
 */
class Bar extends Foo implements FooInterface, BarInterface {

}

/**
 *
 */
class Baz extends Bar implements FooInterface, BarInterface, BazInterface {

}

/**
 *
 */
class Qux extends Foo implements FooInterface, BarInterface, BazInterface {

}