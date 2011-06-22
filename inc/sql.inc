<?PHP

include('conf.inc');
include('adodb5/adodb.inc.php');

define('QQSQL_DEFAULT_LIMIT', '50');

final class QQSQLEngine
{
	private $db = null;

	private $cache = array();

	public function __construct()
	{
	}

	private function db_connect()
	{
		global $conf;

		if(!empty($conf['db_dsn']))
		{
			$this->db = ADONewConnection($conf['db_dsn']);

			if(empty($this->db))
				die('Sorry, could not connect to database.');
		}

		if(empty($this->db))
			return false;
		else
			return true;
	}

	public function aliases_table_exists()
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		if(empty($this->cache['tables']))
			$this->cache['tables'] = $this->db->MetaTables('TABLES');

		if(in_array('aliases', $this->cache['tables']))
			return true;
		else
			return false;
	}

	public function create_aliases_table()
	{
		$dict = NewDataDictionary($this->db);
		$fields = '
			keyword C(20) NOTNULL PRIMARY,
			command X(1024) NOTNULL';
		// read more carefully: C is capped at 255 chars --> X (4000 max)

		$sqlarray = $dict->CreateTableSQL('aliases', $fields);
		$resu = $dict->ExecuteSQLArray($sqlarray);

		if($resu > 0)
			return true;
		else
			return false;
	}

	public function history_table_exists()
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		if(empty($this->cache['tables']))
			$this->cache['tables'] = $this->db->MetaTables('TABLES');

		if(in_array('history', $this->cache['tables']))
			return true;
		else
			return false;
	}

	public function create_history_table()
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$dict = NewDataDictionary($this->db);
		$fields = '
			hit_time T NOTNULL PRIMARY,
			hit_cmd X(1024) NOTNULL';

		$sqlarray = $dict->CreateTableSQL('history', $fields);
		$resu = $dict->ExecuteSQLArray($sqlarray);

		if($resu > 0)
			return true;
		else
			return false;
	}

	public function url_mappings_table_exists()
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		if(empty($this->cache['tables']))
			$this->cache['tables'] = $this->db->MetaTables('TABLES');

		if(in_array('url_mappings', $this->cache['tables']))
			return true;
		else
			return false;
	}

	public function create_url_mappings_table()
	{
		$dict = NewDataDictionary($this->db);
		$fields = '
			keyword C(20) NOTNULL PRIMARY,
			url C(1024) NOTNULL';

		$sqlarray = $dict->CreateTableSQL('url_mappings', $fields);
		$resu = $dict->ExecuteSQLArray($sqlarray);

		if($resu > 0)
			return true;
		else
			return false;
	}

	public function add_history_entry($command)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');
		
		$q = 'INSERT INTO history (hit_time, hit_cmd) VALUES (?,?)';

		$ts = $this->db->DBTimeStamp(time());
		$resu = $this->db->Execute($q, array($ts, $command));

		if(!empty($resu))
			return true;
		else
			return false;
	}

	public function fetch_history_entries($limit=QQSQL_DEFAULT_LIMIT, $offset=0)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');
		
		$q = 'SELECT hit_cmd,hit_time FROM history ORDER BY hit_time DESC';

		$resu = $this->db->SelectLimit($q, $limit, $offset);

		if(empty($resu))
			return array();
		else
			return $resu->GetArray();
	}

	public function fetch_history_entries_starting_with($text, $limit=QQSQL_DEFAULT_LIMIT, $offset=0)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');
		
		$text .= '%';
		
		$q = 'SELECT hit_cmd,hit_time FROM history WHERE hit_cmd LIKE ? ORDER BY hit_time DESC';

		$resu = $this->db->SelectLimit($q, $limit, $offset, array($text));

		if(empty($resu))
			return array();
		else
			return $resu->GetArray();
	}

	public function count_history_entries()
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');
		
		$q = 'SELECT COUNT(hit_time) FROM history';

		$resu = $this->db->Execute($q);

		if(empty($resu))
			return false;
		else
			return $resu->fields[0];
	}

	public function count_history_entries_starting_with($text)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');
		
		$text .= '%';
		
		$q = 'SELECT COUNT(hit_time) FROM history WHERE hit_cmd LIKE ?';

		$resu = $this->db->Execute($q, array($text));

		if(empty($resu))
			return false;
		else
			return $resu->fields[0];
	}

	public function add_mapping($command, $url)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$q = 'INSERT INTO url_mappings (keyword, url) VALUES (?,?)';

		$resu = $this->db->Execute($q, array($command, $url));

		if(empty($resu))
		{
			// We could use an "ON DUPLICATE KEY UPDATE" clause, but I'm not
			//sure if that even works with anything other than MySQL. So, better
			//safe than sorry.
			$q = 'UPDATE url_mappings SET url = ? WHERE keyword = ?';

			$resu = $this->db->Execute($q, array($url, $command));

			if(!empty($resu))
				return true;

			return false;
		}
		else
			return true;
	}

	public function delete_mapping($keyword)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$q = 'DELETE FROM url_mappings WHERE keyword = ?';

		$resu = $this->db->Execute($q, array($keyword));

		if(empty($resu))
			return false;
		else
			return true;
	}

	public function get_mappings()
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$q = 'SELECT keyword, url FROM url_mappings';

		$resu = $this->db->Execute($q);

		if(empty($resu))
			return array();
		else
			return $resu->GetAssoc();
	}
}

?>
