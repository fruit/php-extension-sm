--TEST--
Check dice coefficient calculation results
--SKIPIF--
<?php if (!extension_loaded("sm")) die("Extension sm not loaded"); ?>
--FILE--
<?php
// empty lines are equal (1.00)
printf("%.2f\n", strike_match("", ""));

// completely different strings (0.00)
printf("%.2f\n", strike_match("", "PHP"));

// symbol case matters (0.00)
printf("%.2f\n", strike_match("php", "PHP"));

// matches only "language" words (0.78)
// chunks [ph/hp la/an/ng/gu/ua/ag/ge] and [la/an/ng/gu/ua/ag/ge PH/HP] = 7 * 2.0 / (2+7 + 7+2)
printf("%.2f\n", strike_match("php language", "language PHP"));

// Single symbols has no effects. Comparing "php" with "php". (1.00)
printf("%.2f\n", strike_match("php 5", "php 6"));

// Single symbols has no effects. Comparing "php" with "php6". (0.80)
printf("%.2f\n", strike_match("php 5", "php6"));

// chunks: ph/hp and ph/hp/p6 = (2 * 2.0) / (2 + 3) = (0.80)
printf("%.2f\n", strike_match("php", "php6"));
?>
--EXPECT--
1.00
0.00
0.00
0.78
1.00
0.80
0.80
