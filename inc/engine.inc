<?PHP
// engine.inc -- the driving engine behind the QQSearch application

final class QQEngine
{
	private $sql;

	public function __construct()
	{
		$this->sql = new QQSQLEngine(true);
	}

	public $have_aliases = true;
	public $qq_builtins = array();
	private $ARGS;

	public function parse_args($query_string)
	{
		$this->ARGS = array();

		if(!empty($query_string))
			$this->ARGS = str_getcsv($query_string, ' ');
	}

	private function get_command()
	{
		$resu = null;
		if(!empty($this->ARGS[0]))
			$resu = $this->ARGS[0];

		return $resu;
	}

	private function have_command($command)
	{
		if(!empty($this->qq_builtins[$command]))
			return true;
		else
			return false;
	}

	private function do_command($command)
	{
		$this->qq_builtins[$command]->execute($this->ARGS);

	}

	private function qq_get_builtins()
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

	public function run()
	{
		if(!$this->sql->aliases_table_exists())
			$this->have_aliases = false;

		$this->qq_builtins = $this->qq_get_builtins();

		$this->parse_args($_GET['q']);

		$command = $this->get_command();

		if($this->have_command($command))
			$this->do_command($command);
		else
			die('Couldn\'t find "'.$command.'", sorry.');
	}
}
?>
