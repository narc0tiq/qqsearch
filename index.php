<?PHP
// QQSearch endpoint. Uses QQEngine to do the real work.

define('PATH_BASE', dirname(__FILE__));

// - includes
include('inc/base_class.inc');
include('inc/base.inc');
include('inc/sql.inc');
include('inc/engine.inc');

$engine = new QQEngine;

$engine->run();

?>
