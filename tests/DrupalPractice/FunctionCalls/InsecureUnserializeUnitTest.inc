<?php

// Secure usages should have no errors.
unserialize($foo, ['allowed_classes' => FALSE]);
unserialize($foo, [
  'allowed_classes' => FALSE
]);
unserialize($foo, array('allowed_classes' => FALSE));
unserialize($foo, ['allowed_classes' => ['Foo']]);
unserialize($foo, ['allowed_classes' => ['Foo', 'Bar']]);

// Insecure usages of unserialize should all have one error.
unserialize($foo, ['allowed_classes' => TRUE]);
unserialize($foo, [
  'foo' => 'bar',
]
);
unserialize($foo);
unserialize(
  $foo
);
unserialize($foo, ['allowed_classes']);

// This is safe but the sniff isn't smart enough to figure it out.
$allowed = ['allowed_classes' => FALSE];
unserialize($foo, $allowed);
