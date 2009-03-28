<?PHP
// help.inc -- help built-in

class QQBuiltinHelp extends QQBuiltinCommand
{
	function __construct()
	{
		// Get my short name from the current filename.
		$this->name = basename(__FILE__,'.inc');
		$this->shortdesc = 'Returns helpful information about any supported keyword.';
		$this->longdesc = <<<EOS
Usage: {$this->name} [KEYWORD]
Returns helpful information about KEYWORD.

KEYWORD may be a built-in command, a URL mapping, or an alias. If KEYWORD is
	omitted, {$this->name} will return information about itself.

See also:
	<a href="?q=help+list">list</a>, <a href="?q=help+alias">alias</a>.
EOS;
	}

	function execute($_ARGS)
	{
		$keyword = $this->name;
		if(!empty($_ARGS[1]))
			$keyword = $_ARGS[1];

		global $engine;

		if($engine->have_command($keyword))
			echo $engine->get_command($keyword)->get_longdesc();
		elseif($engine->have_url_mapping($keyword))
		{
			echo '"'.$keyword.'" is a URL mapping to:'."\n\t".$engine->get_url_mapping($keyword);
		}
		else
			echo 'Sorry, "'.$keyword.'" does not seem to be a valid keyword.';
	}
}

$my_obj = new QQBuiltinHelp();
$qq_builtins[$my_obj->get_name()] = $my_obj;
?>
