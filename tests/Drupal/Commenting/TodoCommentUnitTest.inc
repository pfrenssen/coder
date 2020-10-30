<?php

/**
 * @file
 * Test file for the todo standard.
 *
 * These are valid examples.
 *
 * @todo Valid.
 * @todo valid with lower-case first letter
 * @todo $can start with a $
 * @todo \also with backslash
 *
 * These are all incorrect but can be fixed automatically.
 *
 * @TODO Error
 * @ToDo Error
 * @TODo Error
 * @ToDO Error
 * @todo: Error
 * @todo : Error
 * @to-do Error
 * @TO-DO Error
 * @To-Do Error
 * @TO do Error
 * @to  do Error
 * @todo- Error
 * @todo - Error
 * @todoError
 * @todo   Error
 * todo Error
 * TODO Error
 * ToDo Error
 * to-do Error
 *
 * These are all incorrect but cannot be fully fixed automatically.
 *
 * @todo
 * @to-do
 * @TODO
 */

/**
 * Example function.
 */
function foo() {
  // These are valid examples.
  // @todo Valid.
  // @todo valid with lower-case first letter
  // @todo $can start with a $
  // @todo \also with backslash
  // This is not a todo tag. It is a general comment and we do not want
  // to do the standards checking here.
  // These are all incorrect but can be fixed automatically.
  // @TODO Error
  // @ToDo Error
  // @TODo Error
  // @ToDO Error
  // @todo: Error
  // @todo : Error
  // @to-do Error
  // @TO-DO Error
  // @To-Do Error
  // @TO do Error
  // @to    do Error
  // @todo   Error
  // @todo- Error
  // @todo - Error
  // @todoError
  // todo Error
  // TODO Error
  // ToDo Error
  // to-do Error
  // These are all incorrect but cannot be fully fixed automatically.
  // @todo
  // @to-do
  // @TODO
}
