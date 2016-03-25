--TEST--
Check return type of strike_match
--SKIPIF--
<?php if (!extension_loaded("sm")) die("Extension sm not loaded"); ?>
--FILE--
<?php
echo gettype(strike_match("php", "PHP")), PHP_EOL;
echo gettype(strike_match("php var", "PHP foo")), PHP_EOL;
?>
--EXPECT--
double
double