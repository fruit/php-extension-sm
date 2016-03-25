--TEST--
When nothing passed to function
--SKIPIF--
<?php if (!extension_loaded("sm")) die("Extension sm not loaded"); ?>
--FILE--
<?php
echo strike_match();
echo strike_match('arg text one');
echo strike_match('arg text one', 'arg text two', 'arg text three');
?>
--EXPECTF--
Warning: strike_match() expects exactly 2 parameters, 0 given in %s

Warning: strike_match() expects exactly 2 parameters, 1 given in %s

Warning: strike_match() expects exactly 2 parameters, 3 given in %s