<?PHP
// base.inc -- defines basic functions required by QQSearch

if (!function_exists('str_getcsv'))
{
	function str_getcsv($input, $delimiter = ",", $enclosure = '"', $escape = "\\")
	{
		$mem_limit = 5 * 1024 * 1024;

		if(strlen($input) > $mem_limit)
			die('Fatal error in str_getcsv: buffer would overflow past limit of '.$mem_limit.' bytes.');

		$fp = fopen("php://temp/maxmemory:$mem_limit", 'r+');
		fputs($fp, $input);
		rewind($fp);

		$data = fgetcsv($fp, 0, $delimiter, $enclosure); //  $escape only got added in 5.3.0

		fclose($fp);
		return $data;
	}
}

?>
