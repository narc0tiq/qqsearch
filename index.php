<?PHP
// Primary driver for the QQSearch. Currently, just pseudo-logic.

// - includes
include('inc/base_class.inc');

// Testing the skeleton:
$qq_builtins = array();
include('builtins/skel.inc');

foreach($qq_builtins as $b)
{
	$b->execute(null);
}

// - initialize:
// -- check for databases
// -- disable aliases if they're missing
// -- complain if there's no database at all
// -- include the builtins
// - check for a command; if none, default to "none"
// - parse the command
// - check what should be called for it and call it
// - finalize?

?>
