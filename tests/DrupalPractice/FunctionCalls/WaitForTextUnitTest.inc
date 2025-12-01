<?php

// Correct assertion
$this->assertTrue($this->assertSession()->waitForText('Example Text'));
$this->assertFalse($this->assertSession()->waitForText('Example Text'));

// Assigned to a variable which we assume will be asserted later
$wait = $this->assertSession()->waitForText('Example Text');

// Incorrect assertion, waitForText only returns true or false and should be asserted as such
$this->assertEmpty($this->assertSession()->waitForText('Example Text'));

// No Assertion, failure
$this->assertSession()->waitForText('Example Text');

