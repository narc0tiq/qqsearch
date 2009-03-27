<?PHP
// Primary driver for the QQSearch. Currently, just pseudo-logic.

define('PATH_BASE', dirname(__FILE__));

// - includes
include('inc/base_class.inc');
include('inc/base.inc');

// - initialize:
// -- check for databases
// -- disable aliases if they're missing
// -- complain if there's no database at all
// -- include the builtins
$qq_builtins = qq_get_builtins();

// - check for a command; if none, default to "none"
// - parse the command
// - check what should be called for it and call it
// - finalize?

?>
