<?PHP
// del.inc -- built-in for deleting url mappings

class QQBuiltinDel extends QQBuiltinCommand
{
	function __construct()
	{
		// Get my short name from the current filename.
		$this->name = basename(__FILE__,'.inc');
		$this->shortdesc = 'Deletes URL mappings.';
		$this->longdesc = <<<EOS
Usage: {$this->name} KEYWORD [KEYWORD [...]]
Deletes the URL mapping for KEYWORD (or a list of keywords)

KEYWORD must be a URL mapping -- it may not be an alias or a built-in command.
	If KEYWORD does not exist, a warning will be given.

See also:
	<a href="?q=help+add">add</a>, <a href="?q=help+unalias">unalias</a>.
EOS;
	}

	public function execute($_ARGS)
	//*f This is the workhorse function of the skel built-in.
	//* Normally, you'd put your clever whiz-bang smart thing here.
	{
		if(count($_ARGS) < 2)
			return;

		$slice = array_slice($_ARGS, 1);

		global $engine;
		foreach($slice as $keyword)
		{
			if($engine->have_url_mapping($keyword))
			{
				if(!$engine->delete_url_mapping($keyword))
					echo 'Warning: Could not delete URL mapping for "'.$keyword.'"'."\n";
			}
			else
				echo 'Warning: "'.$keyword.'" does not exist or is not a URL mapping.'."\n";
		}
	}
}

$my_obj = new QQBuiltinDel();
$qq_builtins[$my_obj->get_name()] = $my_obj;
?>
