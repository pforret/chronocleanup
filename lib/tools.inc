<?php
include_once("class_cache.inc");
$debug=false;
$time_scriptstarted=microtime(true);

function trace($info,$level="DEBUG"){
	global $debug;
	global $time_scriptstarted;
	$level=strtoupper($level);
	if(!$debug AND $level=="DEBUG"){
		// only print info if in debug mode
		return false;
	}
	$secs=round(microtime(true) - $time_scriptstarted,3);
	if(is_array($info)){
		if(PHP_SAPI=="cli"){
			printf("#- %s @ %06.3fs:\n",$level,$secs);
			print_r($info);
			echo "#-----\n";
		} else {
			// html formatted
			printf("\n<-- %s @ %06.3fs:\n",$level,$secs);
			print_r($info);
			echo "\n-->\n";
		}
	} else {
		if(PHP_SAPI=="cli"){
			printf("#- %s @ %06.3fs: %s\n",$level,$secs,$info);
		} else {
			printf("\n<-- %s @ %06.3fs: %s -->\n",$level,$secs,$info);
		}
	}
}

function warning($scope,$message,$exit=false){
	if($exit){
		$errmsg=sprintf("%8s: WARNING: %s: %s",date("H:i:s"),$scope,$message);
		error_log($errmsg);
		error_log("*** EXIT");
		exit(1);
	} else {
		$errmsg=sprintf("%8s: ERROR  : %s: %s",date("H:i:s"),$scope,$message);
		error_log($errmsg);
	}
}

function progress($scope,$message){
	$errmsg=sprintf("%8s: %s: %s",date("H:i:s"),$scope,$message);
	error_log($errmsg);
}

function cmdline($text,$folder=false,$cachesecs=30){
	if($folder){
		$path=realpath($folder);
		if(!$path){
			trace("cannot find folder [$folder]","ERROR");
			return false;
		}
		$line="cd \"$path\"; $text";
	} else {
		$line=$text;
	}
	if($cachesecs>0){
		$cc=New Cache;
		$result=$cc->get_arr($line,"bash",$cachesecs);
		trace("cmdline: [$line] = " . count($result) . " from cache");
		if($result)	return $result;
	}
	$stdout=Array();
	$result=exec("$line",$stdout);
	trace("cmdline: [$line] = " . count($stdout) . " lines returned");
	if($cachesecs>0 AND $stdout){
		$cc=New Cache;
		$cc->set_arr($line,"bash",$stdout);
	}
	return $stdout;
}

function getparam($name,$default=false){
	if(isset($_GET[$name])){
		$value=$_GET[$name];
		trace("getparam: [$name] = [" . htmlspecialchars(substr($value,0,20)) . "]");
		return $value;
	}
	$value=$default;
	trace("getparam: [$name] = [" . htmlspecialchars(substr($value,0,20)) . "] (default)");
	return $value;
}

function preg_find($pattern,$subject){
	preg_match($pattern,$subject,$matches);
	if($matches){
		trace($matches);
		return $matches[0][1];
	} else {
		trace("preg_find: $pattern not found");
		return false;
	}
}

function contains($haystack,$needle){
	if(!$needle) return false;
	$pos=strpos($haystack,$needle);
	if($pos>0) return true; // in haystack
	if($pos===false) return false; // not in
	if($pos===0) return true; // starts with
	return false; //normally never
}


function listfiles($folder,$ext="",$recursive=false){
	//trace("listfiles: Reading [$ext] files from folder [$folder]");
	if(!$folder){
		trace("listfiles WARNING: start folder cannot be empty");
		return false;
	}
	if(!file_exists($folder)){
		trace("listfiles WARNING: folder [$folder] does not exist");
		return false;
	}
	if(!is_dir($folder)){
		trace("listfiles WARNING: [$folder] is not a folder");
		return false;
	}	
	$extlen=strlen($ext);
	$selection=Array();
	$cfiles=scandir($folder);
	if($cfiles){
		foreach($cfiles as $cfile){
			$cfull="$folder/$cfile";
			$selected=true;
			if(!is_file($cfull)) $selected=false;
			if($ext AND substr($cfile,strlen($cfile)-$extlen,$extlen) <> $ext) $selected=false;
			//trace("listfiles: [$cfull] : $selected");
			if($selected)	$selection[]=$cfull;
		}
		trace("listfiles: [$ext] files in [$folder] = " . count($selection));
	} else {
		trace("WARNING: no files found in [$folder]");
	}
	return $selection;
}

?>
