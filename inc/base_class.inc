<?PHP
// base_class.inc -- The QQBuiltinCommand abstract base class

abstract class QQBuiltinCommand
{
	abstract function __construct();
	abstract function execute($_ARGS);

	protected $name; // is used as the command keyword
	protected $shortdesc; // is shown in the "ls" listing
	protected $longdesc; // is shown in the "help" output

	public function get_name() { return $this->name; }
	public function get_shortdesc() { return $this->shortdesc; }
	public function get_longdesc() { return $this->longdesc; }
}

?>
