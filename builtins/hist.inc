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
Usage: {$this->name} [LIMIT [OFFSET]] [STARTS-WITH]
Display the history of commands that have been used on this instance of
QQSearch while history recording was on (by default, it is off).

LIMIT specifies the number of entries to show on each page of the history
	listing. This parameter is optional and will be set to 30 if not given.

OFFSET specifies the offset from which to start displaying up to LIMIT entries.
	If not given, it is assumed to be 0 (the first page of entries).

STARTS-WITH specifies a filter that will only show those history entries that
	start with the string STARTS-WITH.
	Note: If STARTS-WITH starts with a numeric character, both LIMIT and OFFSET
	must be provided. If it starts with an alphabetical character, they may be
	omitted.

Examples:
	hist -- will display 30 entries per page, showing the first page.
	hist 50 -- will display 50 entries per page, showing the first page.
	hist 50 75 -- will display 50 entries starting with the 76th.
	hist 30 0 something -- will display the first 30 entries whose command
		line starts with "something", e.g. "something neat".

Notes:
 * To enable history logging, use the command "add history on". To disable
it again, use either "add history off" or "del history".
 * The "hist" command will never be logged (to prevent spamming you with
irrelevant details), however all other commands will.
EOS;
	}

	private function paginate($limit, $offset, $count, $prefix='?q=hist',$suffix='')
	{
		$page_count = ceil($count / $limit);
		$cur_page = floor($offset / $limit) + 1;
		$is_exact_page = ($offset % $limit == 0);

		$str = '<pre>';

		$max_entry = min($count, $offset + $limit);
		$str .= 'Showing entries '.($offset + 1).' to '.$max_entry.' of '.$count.'. ';
		$str .= 'Pages: ';

		$displayed_ellipsis = false;
		for($i = 1; $i <= $page_count; $i++)
		{
			if($i > 3 and abs($i - $cur_page) > 2 and $i < ($page_count - 3) and $i %10 != 0)
			{
				if(!$displayed_ellipsis)
					$str .= '... ';

				$displayed_ellipsis = true;
				continue;
			}

			$displayed_ellipsis = false;

			$page_offset = (($i - 1) * $limit);
			$page_url = $prefix.'+'.$limit.'+'.$page_offset.'+'.$suffix;
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
		$str = '';

		for($i = 0; $i < 1; $i++) // "fake" loop to allow break-ing out
		{
			if(!empty($_ARGS[1]))
			{
				if(is_numeric($_ARGS[1]))
					$limit = intval($_ARGS[1]);
				elseif(is_string($_ARGS[1]))
				{
					$slice = array_slice($_ARGS, 1);
					$str = implode(' ', $slice);
					break;
				}
			}
			if(!empty($_ARGS[2]))
			{
				if(is_numeric($_ARGS[2]))
					$offset = intval($_ARGS[2]);
				elseif(is_string($_ARGS[2]))
				{
					$slice = array_slice($_ARGS, 2);
					$str = implode(' ', $slice);
					break;
				}
			}
			if(!empty($_ARGS[3]))
			{
				$slice = array_slice($_ARGS, 3);
				$str = implode(' ', $slice);
			}
		}

		global $engine;
		$count = $engine->count_history($str);
		echo $this->paginate($limit, $offset, $count, '?q=hist', $str);

		$hist_entries = array();
		if(empty($str))
			$hist_entries = $engine->fetch_history($limit, $offset);
		else
			$hist_entries = $engine->fetch_history_entries_starting_with($str, $limit, $offset);

		foreach($hist_entries as $row)
		{
			$row['hit_time'] = date('D Y-m-d H:i:s', strtotime(trim($row['hit_time'], '\'')));
			$row['hit_cmd'] = trim(stripslashes($row['hit_cmd']));
			printf('%-20s <a href="?q=%2$s">%2$s</a>'."\n", $row['hit_time'], htmlentities($row['hit_cmd']));
		}
		echo $this->paginate($limit, $offset, $count, '?q=hist', $str);
	}
}

$my_obj = new QQBuiltinHist();
$qq_builtins[$my_obj->get_name()] = $my_obj;
?>
