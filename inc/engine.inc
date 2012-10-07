<?PHP
// engine.inc -- the driving engine behind the QQSearch application

final class QQEngine
{
	private $sql;

	public function __construct()
	{
		$this->sql = new QQSQLEngine();
	}

	private $have_aliases = false;
	private $have_history = false;
	private $qq_builtins = array();
	private $qq_url_maps = array();
	private $history_count = 0;
	private $ARGS;

	public function parse_args($query)
	{
		$this->ARGS = array();

		if(ini_get('magic_quotes_gpc'))
			$query = stripcslashes($query);

		// The magic of str_getcsv with ' ' as the separator is that it will
		//correctly parse double-quoted strings as a _single_ argument.
		if(!empty($query))
			$this->ARGS = str_getcsv($query, ' ');
	}

	public function get_all_url_mappings()
	{
		if(!empty($this->qq_url_maps))
			return $this->qq_url_maps;
		else
			return array();
	}

	public function have_url_mapping($keyword)
	{
		if(!empty($this->qq_url_maps[$keyword]))
			return true;
		else
			return false;
	}

	public function get_url_mapping($keyword)
	{
		if(!empty($this->qq_url_maps[$keyword]))
			return $this->qq_url_maps[$keyword];
		else
			return false;
	}

	public function delete_url_mapping($keyword)
	{
		return $this->sql->delete_mapping($keyword);
	}

	public function get_all_commands()
	{
		if(!empty($this->qq_builtins))
			return array_keys($this->qq_builtins);
		else
			return array();
	}

	public function get_command($command)
	{
		$resu = null;
		if(!empty($this->qq_builtins[$command]))
			$resu = $this->qq_builtins[$command];

		return $resu;
	}

	public function have_command($command)
	{
		if(!empty($this->qq_builtins[$command]))
			return true;
		else
			return false;
	}

	public function fetch_history($limit=QQSQL_DEFAULT_LIMIT, $offset=0)
	{
		return $this->sql->fetch_history_entries($limit, $offset);
	}

	public function count_history($starts_with = null)
	{
		if(empty($starts_with))
			return $this->history_count;
		else
			return $this->sql->count_history_entries_starting_with($starts_with);
	}

	private function get_input_form($q = '')
	{
		$str = '';

		$q = htmlentities($q);
		$str .= <<<EOS
<h1>QQSearch v0.5 SQLite3</h1>
<form method="get">
	<input type="text" name="q" style="width: 80%" value="$q" id="input-query" />
	<input type="submit" value="submit" style="width: 10%" />
</form>
<div style="position: relative; left: 10%; width: 80%"><a href="?q=list">list</a> <a href="?q=help">help</a></div>
EOS;

		if($this->have_history)
		{
			$str .= <<<EOS
<div style="position: absolute; right: 5px; top: 5px">
	<a href="?q=hist">History</a> enabled; {$this->history_count} commands remembered
</div>
EOS;
		}

		return $str;
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

	public function fetch_history_entries_starting_with($text, $limit=QQSQL_DEFAULT_LIMIT, $offset=0)
	{
		return $this->sql->fetch_history_entries_starting_with($text, $limit, $offset);
	}

	public function run($query = null)
	{
		if(!$this->sql->table_exists('aliases')
		   and !$this->sql->create_aliases_table())
			$this->have_aliases = false;

		if(!$this->sql->table_exists('url_mappings')
		   and !$this->sql->create_url_mappings_table())
			die('Fatal error: could not create the url_mappings table.');

		$this->qq_builtins = $this->qq_get_builtins();
		$this->qq_url_maps = $this->sql->get_mappings();

		if(!empty($this->qq_url_maps['history']) and ($this->qq_url_maps['history'] == 'on')
		  and ($this->sql->table_exists('history') or $this->sql->create_history_table()))
			$this->have_history = true;
		else
			$this->have_history = false;

		if(!empty($this->have_history))
			$this->history_count = $this->sql->count_history_entries();

		$query = trim($query);
		$this->parse_args($query);

		if($this->have_history and !empty($query))
		{
			// Don't add "hist" commands; do add everything else
			if(($this->ARGS[0] != 'hist')
			  and ($this->ARGS[0] != 'suggest')
			  and ($this->ARGS[0] != 'list')
			  and ($this->ARGS[0] != 'ls')
			)
				$this->sql->add_history_entry($query);
		}

		$keyword = '';
		if(!empty($this->ARGS[0]))
			$keyword = $this->ARGS[0];

		if(empty($keyword))
			echo $this->get_input_form();
		elseif($this->have_command($keyword))
			$this->do_command($keyword);
		elseif($this->have_url_mapping($keyword))
			$this->do_url_mapping($keyword);
		else
		{
			echo $this->get_input_form();
			die('<pre>Couldn\'t find "'.$keyword.'", sorry.</pre>');
		}
	}

	private function do_url_mapping($keyword)
	{
		$url = $this->get_url_mapping($keyword);

		$url = $this->interpolate_arguments($url, $this->ARGS);

		header('Location: '.$url);
		die();
	}

	private function interpolate_arguments($str, $args)
	{
		header('Content-Type: text/plain');
		$matches = array();
		preg_match_all('/{{([0-9-*]+)}}/', $str, $matches, PREG_PATTERN_ORDER);

		foreach($matches[1] as $m)
		{
			$pieces = explode('-', $m);

			if(count($pieces) == 2)
			{
				$start = $pieces[0];
				$end = $pieces[1];

				if(empty($start))
					$start = 1;
				if(empty($end))
					$end = count($args) - 1;

				$slice = array_slice($args, $start, ($end - $start) + 1);

				$trans = $this->merge_array_slice_for_url($slice);

				$str = @str_replace('{{'.$m.'}}', $trans, $str);
			}
			else
			{
				if($m == '*')
				{
					$slice = array_slice($args, 1);
					$trans = $this->merge_array_slice_for_url($slice);

					$str = @str_replace('{{*}}', $trans, $str);
				}
				elseif(is_numeric($m))
				{
					$str = @str_replace('{{'.$m.'}}', $this->merge_array_slice_for_url($args[$m]), $str);
				}
			}
		}

		return $str;
	}

	private function merge_array_slice_for_url($slice)
	{
		if(!is_array($slice))
			return urlencode($slice);

		foreach($slice as $k => $v)
		{
			if(strpos($v, ' ') !== false)
				$v = '"'.$v.'"';

			$slice[$k] = urlencode($v);
		}

		return implode('+', $slice);
	}

	public function add_mapping($command, $url)
	{
		return $this->sql->add_mapping($command, $url);
	}
}
?>
