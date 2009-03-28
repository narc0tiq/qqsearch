<?PHP
// list.inc -- built-in for listing other commands (currently only other builtins)

class QQBuiltinList extends QQBuiltinCommand
{
	function __construct()
	{
		// Get my short name from the current filename.
		$this->name = basename(__FILE__,'.inc');
		$this->shortdesc = 'Lists all defined keywords, along with their short description.';
		$this->longdesc = <<<EOS
Usage: {$this->name}
Lists all defined keywords, along with their short description.

See also:
	<a href="?q=help+help">help</a>
EOS;
	}

	function execute($_ARGS)
	{
		global $engine;
		$commands = $engine->get_all_commands();
		$url_maps = $engine->get_all_url_mappings();

		echo 'Built-in commands:'."\n";
		foreach($commands as $c)
		{
			$desc = $engine->get_command($c)->get_shortdesc();

			$str = sprintf('%-20s %s'."\n", $c, $desc);
			$str = wordwrap($str, 80, "\n\t\t");
			echo $str;
		}

		echo "\n".'URL Mappings:'."\n";
		foreach($url_maps as $k => $u)
		{
			$str = sprintf('%-20s %s'."\n", $k, $u);
			$str = wordwrap($str, 80, "\n\t\t");
			echo $str;
		}
	}
}

$my_obj = new QQBuiltinList();
$qq_builtins[$my_obj->get_name()] = $my_obj;
?>
