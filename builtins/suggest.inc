<?PHP
// suggest.inc -- JSON suggestions built-in

class QQBuiltinSuggest extends QQBuiltinCommand
{
	function __construct()
	{
		// Get my short name from the current filename.
		$this->name = basename(__FILE__,'.inc');
		$this->shortdesc = 'JSON suggestions built-in command for QQSearch. Supports the search bar addon (see install).';
		$this->longdesc = <<<EOS
Usage: {$this->name} KEYWORD [KEYWORD [...]]
Returns a set of suggestions based on a given query string in JSON format,
compatible with custom search providers for Firefox and IE7+.

Basically, you don't really need to worry about it, it's just there to help the
machines figure stuff out.
EOS;
	}

	public function execute($_ARGS)
	//*f This is the workhorse function of the skel built-in.
	//* Normally, you'd put your clever whiz-bang smart thing here.
	{
		ob_end_clean();
		header('Content-Type: application/x-suggestions+json');

		unset($_ARGS[0]);
		$query = implode(' ', $_ARGS);

		global $engine;
		$commands = $engine->get_all_commands();
		$mappings = $engine->get_all_url_mappings();

		$hist_entries = array();
		if(strlen($query) >= 3)
			$hist_entries = $engine->fetch_history_entries_starting_with($query,5);

		$ret = array();
		$descriptions = array();
		foreach($commands as $c)
			if(strpos($c, $query) === 0)
			{
				$ret[] = $c;
				$descriptions[] = $c.' [command]';
			}

		foreach($mappings as $k=>$m)
		{
			if(strpos($k, $query) === 0)
			{
				$ret[] = $k;
				$descriptions[] = $k.' -> '.$m;
			}
		}

		foreach($hist_entries as $h)
		{
			$ret[] = $h['hit_cmd'];
			$descriptions[] = trim($h['hit_time'],"'");
		}
		
		$ret[] = 'hist 30 0 '.$query;
		$descriptions[] = 'More from history...';

		$resu = array($query,$ret,$descriptions);
		echo json_encode($resu);
		exit();
	}
}

$my_obj = new QQBuiltinSuggest();
$qq_builtins[$my_obj->get_name()] = $my_obj;
?>
