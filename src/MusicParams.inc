<?php
//
// Music Library
// Tools and to maintain music library
//

//
// Parameters
//

//
// Host-based Param Section
//  Params can change as hostname changes
//
// If a param is not set when we get to defaults, it is set
//  using $homedir
//
// Forward slash should work on windows and linux
// OStype is a system global that should be set with each host
//

//
// Default params (which can be overwritten by host-specification below)
//
// OStype  Values: Windows, Linux
$OStype="Windows";

////////////// ========================================================
//
// Host-based Params
//
// get $hostname to determine host-based parameters
$hostname = gethostname();

if ($hostname == "ProdHost")
{
  // Production Host

  // $homedir - for systems that are built around a single home directory
  $homedir="E:\MusicLibrary";

  // source directory of wave files and cue files
  $srcdir = "e:/MusicLibrary/Music Ref";

  // output/conversion top directory.  Types of conversions usually housed
  //  bloew this directory
  $conversion_base_dir = "e:/MusicLibrary/Conversion";

}
elseif ($hostname == "Rising-Sun")  {

  // $homedir - for systems that are built around a single home directory
  $homedir="D:/MusicLibrary";

  // source directory of wave files and cue files
  $srcdir = "N:/QT";
  //  $srcdir = "N:/QT-Rip4/Succeed";
  //$srcdir = "N:/Q-Ref1-1";
  // $srcdir = "N:/QX1-1";

  // output/conversion top directory.  Types of conversions usually housed
  //  below this directory
//  $conversion_base_dir = "D:/Music/conversion";
  $conversion_base_dir = "N:/Q-Out";

}
elseif ($hostname == "Hector")  {
  // hector development machine

  // $homedir - for systems that are built around a single home directory
  $homedir="D:/Quentin/MusicProgramming";

}
elseif ($hostname == "shigoto")  {

  // $homedir - for systems that are built around a single home directory
  $homedir="C:/jlv/MusicLibrary";

  // source directory of wave files and cue files
  //
  // default
  $srcdir = $homedir . "/SrcDir";

  // working versions:
  //$srcdir = "N:/A";
  //$srcdir = "N:/B";
  //$srcdir = "N:/Q-Ref";
  //$srcdir = "N:/QT";
//  $srcdir = "F:/QT6";
//  $srcdir = "F:/Q-Ref1-1";

  // output/conversion top directory.  Types of conversions usually housed
  //  below this directory
//  $conversion_base_dir = "D:/Music/conversion";
  $conversion_base_dir = "F:/Q-Out";

}
elseif ($hostname == "tonto") {
  // tonto development machine

  // $homedir - for systems that are built around a single home directory
  $homedir="C:Quentin\MusicLibrary";

} else {
  // default system home directory with no specification

  // $homedir - for systems that are built around a single home directory
  $homedir="C:\MusicLibrary";

}

////////////// ========================================================

//
// Default parameters if not set on a host basis
//

// OStype - default to windows
if (! isset($OStype)) $OStype="Windows";

// $HomeDir - for systems that are built around a single home directory
if (! isset($homedir)) $homedir="C:\MusicLibrary";

// Log file directory, as default
if (! isset($logdir)) $logdir=$homedir . "/Log";

// source directory of wave files and cue files, as default
if (! isset($srcdir)) $srcdir=$homedir . "/MusicWav";

// output/conversion top directory.  Types of conversions usually housed
//  beloww this directory
if (! isset($conversion_base_dir)) $conversion_base_dir=$homedir . "/Conversion";

// trash directory, as default
if (! isset($trash_name)) $trash_name = "Z Old Cue Sheets";

//
// other parameters, non-host-based
//

// location of coverter binary to be invoked through system call
if (! isset($dflt_converter)) $dflt_converter = "C:\Apps\xrecode3\xrecode3cx64.exe";


// file name to designate cue ripping level directory
$cue_rip_dir_tag = "Z CueRipDir.tag";

//
// Converter parameter array
//
// This array drives how files are converted. The overall array contained
//   one assoc array per conversion type. The following elements must
//   be defined.
//    ID - Unique name for the conversion used in various places. Ex: mp3-192
//    convert - true/false - Flag to turn on/off conversion
//    converter - path to converter exe. The conversion command line is formed
//                of the following:
//                  $converter $pre-cue $cuefile $pre-out $output_dir $post-out
//    base_folder - top-of-tree base folder to place converted filesize
//    cd_dir - TRUE/FALSE - if true, routine cd's to src directory before
//              issueing conversion
//    pre-cue,pre-out,post-out - strings per format above
//    image - true/false - true to include folder.jpg in target directory
//
// $conversions["ID"] = array("convert"=>TRUE, "converter"=>$dflt_converter,
//      "base_folder"=>$dflt_converion_base . "/X", "cd_dir"=>TRUE,
//      "pre-cue"=>"", "pre-out"=>"", "post-out"=>"",
//      "image"=>FALSE);

$conversions=array();

// mp3-128 converter
$conversions["mp3-128"] = array("convert"=>TRUE, "converter"=>$dflt_converter,
      "base_folder"=>$conversion_base_dir . "/MP3-128", "cd_dir"=>TRUE,
      "pre-cue"=>" /dest mp3 /cbr 128 -i ",
      "pre-out"=>" /usesource /pfilename \"%track% %title%\" -o ",
      "post-out"=>"",
      "image"=>FALSE);

// // mp3-192 converter
$conversions["mp3-192"] = array("convert"=>TRUE, "converter"=>$dflt_converter,
      "base_folder"=>$conversion_base_dir . "/MP3-192", "cd_dir"=>TRUE,
      "pre-cue"=>" /dest mp3 /cbr 192 -i ",
      "pre-out"=>" /usesource /pfilename \"%track% %title%\" -o ", "post-out"=>"",
      "image"=>FALSE);

// mp3-320 converter
$conversions["mp3-320"] = array("convert"=>TRUE, "converter"=>$dflt_converter,
      "base_folder"=>$conversion_base_dir . "/MP3-320", "cd_dir"=>TRUE,
      "pre-cue"=>" /dest mp3 /cbr 320 -i ",
      "pre-out"=>" /usesource /pfilename \"%track% %title%\" -o ", "post-out"=>"",
      "image"=>FALSE);


// parameters to feed converter
$convparams = "/dest mp3 /cbr 192 /pfilename %track2%";

// cue file directory depth: cuefiles expected for this depth or greater
//   Example: genre/artist/album is cue_file_dir_depth = 3
$cue_file_dir_depth = 3;


// default log root and optoinally set default ctl string (see log_init for doc)
$log_default_name = "MsLib-Generic";
$log_default_options = "echo[error],echo[info]";

//
// Timezone setting
//  -- the string must come from a PHP list found here:
//       http://php.net/manual/en/timezones.php
$sys_time_zone='America/Los_Angeles';

//
// isDryRun - used by ServerFix to trial run vs. real thing
//
$isDryRun=false;
//$isDryRun=true;

// end of MusicParams.inc
?>
