<?PHP
// add.inc -- built-in for adding commands

class QQBuiltinAdd extends QQBuiltinCommand
{
	function __construct()
	{
		// Get my short name from the current filename.
		$this->name = basename(__FILE__,'.inc');
		$this->shortdesc = 'Adds or replaces a keyword->url mapping (not an alias) in QQSearch.';
		$this->longdesc = <<<EOS
Usage: {$this->name} KEYWORD URL
Creates or updates a KEYWORD to point to a URL.

KEYWORD may be any string of up to 20 characters that does not conflict with
	the name of a built-in command. Note that if KEYWORD exists as an alias,
	the alias definition will override the new URL mapping.

URL may be up to 1023 characters long. If URL contains spaces, you can enclose
	it in double quotes, "like this".

	URL may also contain parameter interpolation, in the format {{1}}, {{2}},
	etc. for single parameters, {{1-3}} for sets of parameters, and {{*}} for
	all parameters. For instance, a Google search query would be:
		add google http://www.google.com/search?q={{*}}

See also:
	<a href="?q=help+del">del</a>, <a href="?q=help+alias">alias</a>.
EOS;
	}

	function execute($_ARGS)
	{
		if(count($_ARGS) != 3)
		{
			echo 'This command takes exactly two parameters.'."\n";
			echo $this->longdesc;
			return false;
		}

		$keyword = $_ARGS[1];
		$url = $_ARGS[2];

		global $engine;
		if($engine->have_command($keyword))
			die('Cannot add "'.$keyword.'", it is already a built-in command.');

		$url = str_replace(' ', '%20', $url);

		if($engine->add_mapping($keyword, $url))
		{
			echo '<b>Success!</b> Added "'.$keyword.'" as "'.$url.'"';
		}
		else
		{
			echo '<b>Failure!</b> Could not add "'.$keyword.'"';
		}
	}
}

$my_obj = new QQBuiltinAdd();
$qq_builtins[$my_obj->get_name()] = $my_obj;
?>
