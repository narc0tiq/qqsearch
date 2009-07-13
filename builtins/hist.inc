<?PHP
// hist.inc -- history displaying built-in

class QQBuiltinHist extends QQBuiltinCommand
{
	function __construct()
	{
		// Get my short name from the current filename.
		$this->name = basename(__FILE__,'.inc');
		$this->shortdesc = 'Displays QQSearch usage history.';
		$this->longdesc = <<<EOS
Usage: {$this->name} [LIMIT] [OFFSET]
Display the history of commands that have been used on this instance of
QQSearch while history recording was on (by default, it is off).

LIMIT specifies the number of entries to show on each page of the history
	listing. This parameter is optional and will be set to 30 if not given.

OFFSET specifies the offset from which to start displaying up to LIMIT entries.
	If not given, it is assumed to be 0 (the first page of entries).

Examples:
	hist -- will display 30 entries per page, showing the first page.
	hist 50 -- will display 50 entries per page, showing the first page.
	hist 50 75 -- will display 50 entries starting with the 76th.

Notes:
 * To enable history logging, use the command "add history on". To disable
it again, use either "add history off" or "del history".
 * The "hist" command will never be logged (to prevent spamming you with
irrelevant details), however all other commands will.
EOS;
	}

	private function paginate($limit, $offset, $count, $prefix='?q=hist')
	{
		$page_count = ceil($count / $limit);
		$cur_page = floor($offset / $limit) + 1;
		$is_exact_page = ($offset % $limit == 0);

		$str = '<pre>';

		$max_entry = min($count, $offset + $limit);
		$str .= 'Showing entries '.($offset + 1).' to '.$max_entry.' of '.$count.'. ';
		$str .= 'Pages: ';
		for($i = 1; $i <= $page_count; $i++)
		{
			$page_offset = (($i - 1) * $limit);
			$page_url = $prefix.'+'.$limit.'+'.$page_offset;
			if($i == $cur_page)
			{
				if($is_exact_page)
					$str .=  '['.$i.']';
				else
					$str .=  '[<a href="'.$page_url.'">'.$i.'</a>]';
			}
			else
				$str .=  '<a href="'.$page_url.'">'.$i.'</a>';

			$str .=  ' ';
		}
		$str .=  "\n</pre>";

		return $str;


	}

	public function execute($_ARGS)
	//*f This is the workhorse function of the skel built-in.
	//* Normally, you'd put your clever whiz-bang smart thing here.
	{
		$limit = 30;
		$offset = 0;

		if(!empty($_ARGS[1]))
			$limit = intval($_ARGS[1]);
		if(!empty($_ARGS[2]))
			$offset = intval($_ARGS[2]);

		global $engine;
		echo $this->paginate($limit, $offset, $engine->count_history());

		$hist_entries = $engine->fetch_history($limit, $offset);

		foreach($hist_entries as $row)
		{
			$row['hit_time'] = trim($row['hit_time'], '\'');
			printf('%-20s <a href="?q=%2$s">%2$s</a>'."\n", $row['hit_time'], $row['hit_cmd']);
		}
		echo $this->paginate($limit, $offset, $engine->count_history());
	}
}

$my_obj = new QQBuiltinHist();
$qq_builtins[$my_obj->get_name()] = $my_obj;
?>
