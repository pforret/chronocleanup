<?php

Class Cache {
	var $enabled=false;

	function __construct($cachedir="cache"){
		if(!file_exists("$cachedir/.")){
			if(mkdir($cachedir,0777,true)){
				trace("created cache dir [$cachedir]");
				$this->enabled=true;
				$this->cachedir=$cachedir;
				return true;
			} else {
				trace("cannot create dir [$cachedir]","ERROR");
				$this->enabled=false;
				return false;
			}
				;
		} else {
			$this->enabled=true;
			$this->cachedir=$cachedir;
			return true;
		}
	}

	function get($id,$group="cache",$maxsecs=295){
		if(!$this->enabled){
			return false;
		}
		$cachefile=$this->mkfilename($id,$group);
		if(!file_exists($cachefile)){
			trace("Cache::get : no cache [$cachefile] yet");
			return false;
		}
		// so the cache file exists
		$age_secs=(time()-filemtime($cachefile));
		if( $age_secs > $maxsecs){
			trace("Cache::get : cache [$cachefile] too old - $age_secs > $maxsecs");
			return false;
		}
		trace("Cache::get : cache [$cachefile] OK - $age_secs <= $maxsecs");
		return file_get_contents($cachefile);
	}

	function get_arr($id,$group="cache",$maxsecs=295){
		if(!$this->enabled){
			return false;
		}
		$cachefile=$this->mkfilename($id,$group);
		if(!file_exists($cachefile)){
			//trace("Cache::get_arr : cache [$cachefile] not found");
			return false;
		}
		// so the cache file exists
		$age_secs=(time()-filemtime($cachefile));
		if( $age_secs > $maxsecs){
			trace("Cache::get_arr : cache [$cachefile] too old - $age_secs > $maxsecs");
			return false;
		}
		trace("Cache::get_arr : cache [$cachefile] OK - $age_secs <= $maxsecs");
		return unserialize(file_get_contents($cachefile));
	}

	function set($id,$group="cache",$value){
		if(!$this->enabled){
			return false;
		}
		if(rand(0,100) > 95){
			$this->cleanup();
		}
		$cachefile=$this->mkfilename($id,$group);
		trace("Cache::set : saving [$cachefile]");
		file_put_contents($cachefile,$value);
		return true;
	}

	function set_arr($id,$group="cache",$array){
		if(!$this->enabled){
			return false;
		}
		if(rand(0,100) > 95){
			$this->cleanup();
		}
		$cachefile=$this->mkfilename($id,$group);
		trace("Cache::set_arr : saving [$cachefile]");
		file_put_contents($cachefile,serialize($array));
		return true;
	}

	function cleanup($hours=24){
		if(!$this->enabled){
			return false;
		}
		$cachefiles=glob("$this->cachedir/*.temp");
		if($cachefiles){
			$treshold=time()-$hours*3600;
			$nbdeleted=0;
			foreach($cachefiles as $cachefile){
				if(filemtime($cachefile) < $treshold){
					unlink($cachefile);
					$nbdeleted++;
				}
			}
			if($nbdeleted > 0){
				trace("Cache::cleanup : removed $nbdeleted old cache files");
			} else {
				// no cache files deleted
			}
		} else {
			// no cache files yet, so nothing to do
		}
	}

	private function mkfilename($id,$group="cache"){
		$group=strtolower(substr($group,0,10));
		$begin=preg_replace("#[^a-zA-Z0-9]*#","",$id);
		$begin=substr($begin,0,10);
		$cacheid="$group.$begin." . substr(sha1($id),0,16);
		$cachefile=$this->cachedir . "/$cacheid.temp";
		return $cachefile;
	}
}

?>