--TEST--
Check for SM presence
--SKIPIF--
<?php if (!extension_loaded("sm")) die("Extension sm not loaded"); ?>
--FILE--
<?php
sm_version();
?>
--EXPECT--
PHP SM Version: 2.1.4