<?PHP
// base.inc -- defines basic functions required by QQSearch

// str_getcsv is in the PHP manual and will be implemented starting with 5.3.0.
//Unfortunately, this is being developed on PHP 5.2.6-2ubuntu4.1, thus it is
//necessary to backport this mostly-compatible version using php://temp and
//fgetcsv, whose minimal requirements are 5.1.0 and 4.*, respectively.
if (!function_exists('str_getcsv'))
{
	function str_getcsv($input, $delimiter = ",", $enclosure = '"')
	{
		$fp = fopen('php://temp/', 'r+')
			or die('Need PHP >= 5.1.0, sorry.');

		fputs($fp, $input);
		rewind($fp);

		$data = fgetcsv($fp, 1024, $delimiter, $enclosure);

		fclose($fp);
		return $data;
	}
}

?>
