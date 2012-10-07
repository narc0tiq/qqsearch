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
			$this->db = new PDO($conf['db_dsn']);

			if(empty($this->db))
				die('Sorry, could not connect to database.');
		}

		if(empty($this->db))
			return false;
		else
			return true;
	}

	private function fetch_tables()
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$q = 'SELECT name FROM sqlite_master WHERE type="table"';
		$stmt = $this->db->query($q);

		if(empty($stmt))
			die('Unable to fetch existing table names');

		return $stmt->fetchAll(PDO::FETCH_COLUMN, 0);
	}

	public function table_exists($tablename)
	{
		if(empty($this->cache['tables']))
			$this->cache['tables'] = $this->fetch_tables();

		if(in_array($tablename, $this->cache['tables']))
			return true;
		else
			return false;
	}

	public function create_aliases_table()
	{
		$q = 'CREATE TABLE aliases (
			keyword VARCHAR(20) NOT NULL,
			command VARCHAR(1024) NOT NULL,
			PRIMARY KEY (keyword)
		)';

		$resu = $this->db->query($q);

		if(empty($resu))
			return false;
		else
			return true;
	}

	public function create_history_table()
	{
		$q = 'CREATE TABLE history (
			hit_time DATE NOT NULL,
			hit_cmd VARCHAR(250),
			PRIMARY KEY (hit_time)
		)';

		$resu = $this->db->query($q);

		if(empty($resu))
			return false;
		else
			return true;
	}

	public function create_url_mappings_table()
	{
		$q = 'CREATE TABLE url_mappings (
			keyword VARCHAR(20) NOT NULL,
			url VARCHAR(1024) NOT NULL,
			PRIMARY KEY (keyword)
		)';

		$resu = $this->db->query($q);

		if(empty($resu))
			return false;
		else
			return true;
	}

	public function add_history_entry($command)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$q = 'INSERT INTO history (hit_time, hit_cmd) VALUES (?,?)';
		$stmt = $this->db->prepare($q);

		$resu = $stmt->execute(array(date('Y-m-d H:i:s'), $command));

		if(!empty($resu))
			return true;
		else
			return false;
	}

	public function fetch_history_entries($limit=QQSQL_DEFAULT_LIMIT, $offset=0)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$q = 'SELECT hit_cmd,hit_time FROM history ORDER BY hit_time DESC LIMIT ? OFFSET ?';
		$stmt = $this->db->prepare($q);

		$resu = $stmt->execute(array($limit, $offset));

		if(empty($resu))
			return array();
		else
			return $stmt->fetchAll(PDO::FETCH_ASSOC);
	}

	public function fetch_history_entries_starting_with($text, $limit=QQSQL_DEFAULT_LIMIT, $offset=0)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$text .= '%';

		$q = 'SELECT hit_cmd,hit_time FROM history WHERE hit_cmd LIKE ? ORDER BY hit_time DESC LIMIT ? OFFSET ?';
		$stmt = $this->db->prepare($q);

		$resu = $stmt->execute(array($text, $limit, $offset));

		if(empty($resu))
			return array();
		else
			return $stmt->fetchAll(PDO::FETCH_ASSOC);
	}

	public function count_history_entries()
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$q = 'SELECT COUNT(hit_time) FROM history';
		$stmt = $this->db->query($q);

		if(empty($stmt))
			return false;

		$resu = $stmt->fetch(PDO::FETCH_NUM);
		return $resu[0];
	}

	public function count_history_entries_starting_with($text)
	{
		if(empty($this->db) and !$this->db_connect())
			die('Unable to connect to database');

		$text .= '%';

		$q = 'SELECT COUNT(hit_time) FROM history WHERE hit_cmd LIKE ?';
		$stmt = $this->db->prepare($q);

		$resu = $stmt->execute(array($text));
		if(empty($resu))
			return false;

		$resu = $stmt->fetch(PDO::FETCH_NUM);
		return $resu[0];
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
		$stmt = $this->db->query($q);

		if(empty($stmt))
			return array();

		$resu = $stmt->fetchAll(PDO::FETCH_COLUMN | PDO::FETCH_GROUP);
		foreach($resu as $k => $v) { $resu[$k] = $v[0]; }
		return $resu;
	}
}

?>
