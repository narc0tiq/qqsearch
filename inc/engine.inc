<?PHP
// engine.inc -- the driving engine behind the QQSearch application

final class QQEngine
{
	private $sql;

	public function __construct()
	{
		$this->sql = new QQSQLEngine(false);
	}

	private $have_aliases = true;
	private $qq_builtins = array();
	private $ARGS;

	public function parse_args($query)
	{
		$this->ARGS = array();

		// The magic of str_getcsv with ' ' as the separator is that it will
		//correctly parse double-quoted strings as a _single_ argument.
		if(!empty($query))
			$this->ARGS = str_getcsv($query, ' ');
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

	private function get_input_form($q)
	{
		return <<<EOS
<form method="get">
	<input type="text" name="q" value="$q" />
	<input type="submit" value="submit" />
</form>
EOS;
	}

	private function do_command($command)
	{
		echo $this->get_input_form(implode(' ', $this->ARGS));
		echo '<pre>'."\n";
		$this->qq_builtins[$command]->execute($this->ARGS);
		echo '</pre>'."\n";
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

	public function run($query = null)
	{
		if(!$this->sql->aliases_table_exists())
			$this->have_aliases = false;

		$this->qq_builtins = $this->qq_get_builtins();

		$this->parse_args($query);

		$command = $this->get_command();

		if($this->have_command($command))
			$this->do_command($command);
		else
			die('Couldn\'t find "'.$command.'", sorry.');
	}

	public function add_command($command, $url)
	{
		return $this->sql->add_command($command, $url);
	}
}
?>