<?php


/*
 * @todo Valid.
 */
$foo = 'bar';

/*
 * @todo Valid
 */
$foo = 'bar';

/*
 * @todo This is valid.
 */
$foo = 'bar';

/*
 * @todo this is valid.
 */
$foo = 'bar';

/*
 * @TODO Error
 */
$foo = 'bar';

/*
 * @ToDo Error
 */
$foo = 'bar';

/*
 * @TODo Error
 */
$foo = 'bar';

/*
 * @ToDO Error
 */
$foo = 'bar';

/*
 * @todo: Error
 */
$foo = 'bar';

/*
 * @to-do Error
 */
$foo = 'bar';

/*
 * @TO-DO Error
 */
$foo = 'bar';

/*
 * @To-Do Error
 */
$foo = 'bar';

/*
 * @TO do Error
 */
$foo = 'bar';

/*
 * @to    do Error
 */
$foo = 'bar';

/*
 * @todo: Error
 */
$foo = 'bar';

/*
 * @todo : Error
 */
$foo = 'bar';

/*
 * @todo- Error
 */
$foo = 'bar';

/*
 * @todo - Error
 */
$foo = 'bar';

/*
 * @todoError
 */
$foo = 'bar';

/*
 * todo Error
 */
$foo = 'bar';

/*
 * TODO Error
 */
$foo = 'bar';

/*
 * ToDo Error
 */
$foo = 'bar';

/*
 * @todo   Error
 */
$foo = 'bar';


/**
 * Test function.
 */
function foo()
{
    // These are valid examples.
    // @todo Valid.
    // @todo Valid
    // @todo This is valid.
    // @todo this is valid.
    $foo = 'bar';

    // These should all be errors.
    // @TODO Error
    // @ToDo Error
    // @TODo Error
    // @ToDO Error
    // @todo: Error
    // @to-do Error
    // @TO-DO Error
    // @To-Do Error
    // @TO do Error
    // @to    do Error
    // @todo: Error
    // @todo : Error
    // @todo- Error
    // @todo - Error
    // @todoError
    // todo Error
    // TODO Error
    // ToDo Error
    // @todo   Error.
    $foo = 'bar';

}//end foo()
