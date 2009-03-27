<?PHP
// skel.inc -- skeleton built-in

class QQBuiltinSkel extends QQBuiltinCommand
{
	function __construct()
	{
		parent::__construct();

		// Get my short name from the current filename.
		$this->name = basename(__FILE__,'.inc');
		$this->shortdesc = 'A skeleton built-in command for QQSearch. Does nothing.';
		$this->longdesc = <<<EOS
{$this->name} is just an example command, showing a pure skeleton built-in.

It does nothing, but its source shows how to implement a built-in command for
QQSearch.
EOS;
	}

	function execute($_ARGS)
	//*f This is the workhorse function of the skel built-in.
	//* Normally, you'd put your clever whiz-bang smart thing here.
	{
		echo 'I do nothing.';
	}
}

$my_obj = new QQBuiltinSkel();
$qq_builtins[$my_obj->get_name()] = $my_obj;
?>
