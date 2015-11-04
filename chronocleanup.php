<?php
include_once("lib/settings.inc");

$shortopts=Array(
	"d"	=> "debug",
	"v"	=> "verbose",
	"?"	=> "help (show usage)",
	);
	
$actions=Array(
	"",
	"test",
	"move",
	);

$prog=str_replace(".php",".sh",realpath($argv[0]));
$progname=basename($argv[0],".php");
$optlist=implode("",array_keys($shortopts));
$options = getopt($optlist);
if(isset($options["d"]))	$debug=true;
if(isset($options["v"]))	$verbose=true;
$raw=$argv;
array_shift($raw); // remove program name itself
$endofopts=false;
while(!$endofopts){
	$vals=array_values($raw);
	$val1=$vals[0];
	if(substr($val1,0,1)=="-"){
		// is an option
		$key=substr($val1,1,1);
		if(isset($shortopts["$key:"])){
			array_shift($raw);
			array_shift($raw);
			continue;
		}
		array_shift($raw);
	} else {
		$endofopts=true;
	}
}
trace("--- Command line:");
trace($argv);
trace("--- Options detected:");
trace($options);
trace("Parameters that are not options:");
trace($raw);

if(isset($options["?"]))	$action="help";
if(!in_array($action,$actions)){
	warning("mrtgobot","[$action] is not an accepted action");
	warning("mrtgobot","Accepted actions (" . implode(",",$actions) . ")",true);
}

switch($action){
case false:
case "help":
case "usage":
	show_usage();
	exit(0);
	break;;
	
case "setup":
	if(!isset($raw[1]) OR !isset($raw[2])){
		warning($action,"need [cfgfolder] and [htmlfolder] as parameters",true);
	}
	$mg->init_ini($raw[1],$raw[2]);
	break;;
	
default:
	warning($progname,"unknown action [$action]",true);
}

function show_usage(){
	global $argv;
	
	echo "
====================================
===== $progname [ACTION] [PARAMS]
===== (c) 2015 - Peter Forret
	
====================================
";
}
?>