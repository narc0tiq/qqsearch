<?PHP
// QQSearch endpoint. Uses QQEngine to do the real work.

error_reporting(E_ALL);
ob_start();

define('PATH_BASE', dirname(__FILE__));

include(PATH_BASE.'/inc/base_class.inc');
include(PATH_BASE.'/inc/base.inc');
include(PATH_BASE.'/inc/sql.inc');
include(PATH_BASE.'/inc/engine.inc');

$engine = new QQEngine;

$query='';
if(!empty($_GET['q']))
	$query = $_GET['q'];
$engine->run($query);

?>
