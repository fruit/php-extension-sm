# Installation

[![Build Status](https://travis-ci.org/fruit/php-extension-sm.svg?branch=master)](https://travis-ci.org/fruit/php-extension-sm)

## Get code from GitHub:

    $ git clone git://github.com/fruit/php-extension-sm.git
    $ cd php-extension-sm
    $ phpize
    $ ./configure --with-sm
    $ make
    # make install

## Update php.ini file

Than add new line to the `php.ini` and restart Apache/PHP-FPM

    extension=sm.so

# Using

    <?php

      # prints: 0.66666666666667
      print strike_match("Hello, World!", "hello world");


# Dependencies

  Version PHP 5.3 or later required