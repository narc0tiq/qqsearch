<?PHP
// base.inc -- defines basic functions required by QQSearch

function qq_get_builtins()
{
	$qq_builtins = array();

	$builtin_files = glob('builtins/*.inc');

	foreach($builtin_files as $f)
	{
		// The includes will automagically populate $qq_builtins as needed
		include($f);
	}

	return $qq_builtins;
}

?>
