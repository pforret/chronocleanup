<?php

Class Cleanup {
	var $enabled=false;

	function __construct($root,$dated="Y/Y-m", $folder="%p", $fname="%f",$guessfromfile=false){
		// in template
		// %p = parent folder name
		// %f = file name
		// Y,m,d = to construct date based

	}
	
	function process_file($path,$mode="test",$guessfromfile=false){
		if($guessfromfile)
		$time=guess_date_from
	}

	private function guess_date_from_folder($path){
		$folder=dirname($path);
		if(isset(this->$timestamps[$folder])){
			return this->$timestamps[$folder];
		} else {
			this->$timestamps[$folder]=$timestamp;
			return this->$timestamps[$folder]
		}
	}
	
	private function guess_date_from_file($path){
		// option 1: file name is like YYYY MM DD
		$bname=basename($path);
		$clean=preg_replace("#([^\d]*)#"," ",$bname); // delete all non digits
		$day=quickmatch($clean);
		
		// option 2: foldername is like YYYY MM DD
		
		// take file creation date
		$timestamp=filectime($path);
	}
	
	function find_date($text,$pattern){
	}
}

?>