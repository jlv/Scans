<?php

// using copy of music library -- do not edit the copy!
require "MusicRequire.inc";

// inline params
$base_dirs = array("L:/Scans/Source of Truth 11-2021/Scans 2 - year folders"
                 );
$missing_dirs = array("L:/Scans/Source of Truth 11-2021/Scans 1",
                    "L:/Scans/Source of Truth 11-2021/Scans - Carolyn Tonto"
                    );

// whole filesystem comparisons
$base_dirs = array("L:/Scans");
$missing_dirs = array("Z:/");


// globals

// $basefiles and missing files -- same structure
//  - indexed on size or size.ascii.ascii
//  attributes: filename, path, extd (flag to indicate there are extended ID's)
$base_files = array();
$missing_files = array();


// load_base index function, called via crawl
//

function load_base($base_folder, $add_folder, $new_base_folder, $filename, $array_of_options) {
  global $base_files;

  logp("echo","File for base: {$add_folder} / {$filename}");

  find_file($base_files, $filename, $base_folder . "/" . $add_folder . "/" . $filename, TRUE);
  return TRUE;

}

// find_missing files function, called via crawl
//  - looks in $base_files array for each file, and if not present, adds to $missing_files array

function find_missing($base_folder, $add_folder, $new_base_folder, $filename, $array_of_options) {
  global $base_files;
  global $missing_files;

  logp("echo","Check for missing: {$add_folder} / {$filename}");

  // look for file in base. If True, do nothing (log)
  if (find_file($base_files, $filename, $base_folder . "/" . $add_folder . "/" . $filename, FALSE) === TRUE)
    logp("log", "Found file in base files: {$add_folder} / {$filename}");
  else
    // check if file is in the missing files already, or add to missing
    if (find_file($missing_files, $filename, $base_folder . "/" . $add_folder . "/" . $filename, TRUE))
      logp("log", array("Missing file already found in missing file list.",
                        " File: {$add_folder} / {$filename}"
                      ));
    else
      logp("log", array("Added to missing file list.",
                      " File: {$add_folder} / {$filename}"
                    ));

  return TRUE;
}

// function find_file
//  returns TRUE if file found in the indexes array, FALSE if not found
//  if $insert is set to TRUE, adds the file and its data to the array if
//    file is not already found in array

function find_file(&$file_array, $filename, $path, $insert=FALSE) {
  // look for size in array
  $filesize = filesize($path);

  // if size exists in array, check if same file
  if (isset($file_array[$filesize]))
    if (files_compare($path, $file_array[$filesize]['path']) === TRUE)
      return TRUE;
    else {
      // not the same file, so check if extended flag set and process
      //  the extended file
      if ( $file_array[$filesize]['extd'] === TRUE) {
        $id = find_file_get_extd_id($path, $filesize);

        // look for file
        if (isset($file_array[$id]))
          if (files_compare($path, $file_array[$id]['path']) === TRUE)
            return TRUE;
          else {
            // error case: extended file was indexed but it's not the same file!
            logp("error",array(
              "ERROR: File was indexed into array but failed file comparison with extended ID.",
              "  File: {$path}",
              "  Filesize: {$filesize}, IndexID: {$id}"
              ));
            return FALSE;
          }
        // else file not in array
        else {
          // if insert, then add file
          if ( $insert === TRUE ) find_file_add_array($file_array, $id, $filename, $path);
          return FALSE;
        }
      // else not extended, so lets add one if $insert
      }
      else {
        if ( $insert === TRUE ) {
          $id = find_file_get_extd_id($path, $filesize);
          find_file_add_array($file_array, $id, $filename, $path);
          $file_array[$filesize]['extd'] = TRUE;

          logp("log","Extended file index {$filesize} for ID {$id}");
        }
        return FALSE;
      }  // end of if extd is true
    } // else files did not compare -- create
  else {
    // if insert, add file
    if ( $insert === TRUE ) find_file_add_array($file_array, $filesize, $filename, $path);
    return FALSE;
  }
} // end of function

// helper function to get extended key
function find_file_get_extd_id($path, $filesize) {
  // constants that deterine byte position for each offset
  $offsets = array(5223, 9101);

  // base id
  $id = $filesize;
  $off_safe = 1;

  // open file
  $fhdl = fopen($path, 'r');

  foreach($offsets as $offset) {
    if ( $offset > $filesize) {
      $offset = $filesize - $off_safe--;
      // check for neg
      if ($offset < 0) $offset = 0;
    }

    // check offset 0, else get byte from file and convert to number
    if ( $offset == 0 ) $id .= ".0";
    else {
      logp("log","offset:{$offset}");
      fseek($fhdl, $offset);
      $id .= "." . ord(fgetc($fhdl));
    }

  }

  // close file
  fclose($fhdl);

  // have id, so return
  return $id;
}


// helper function to add to file array
function find_file_add_array(&$file_array, $arr_key, $filename, $path) {
  $file_array[$arr_key]['filename'] = $filename;
  $file_array[$arr_key]['path'] = $path;
  $file_array[$arr_key]['extd'] = FALSE;

  logp("log", array("Created index for key {$arr_key}",
                    "  File: {$path}"));

  return;
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// Main routine
//
///////////////////////////////////////////////////////////////////////////////////////////

// init log system
logp_init("FileMissing", "");

$options = array();


// load arrays
logp("echo","Loading base files...");
foreach ($base_dirs as $base_dir) {
  crawl($base_dir, "", "", "load_base", $options);
}

//print_r($base_files);

//print "end of test";
//exit();

// Look for missing files
logp("echo","Looking for missing files...");
foreach ($missing_dirs as $missing_dir) {
  crawl($missing_dir, "", "", "find_missing", $options);
}


// output missing dirs
logp("echo","Reporting missing files...");
foreach ($missing_files as $missing_file) {
  logp("info,echo",array(
              "Missing file: {$missing_file['filename']}",
              "   Path: {$missing_file['path']}"
                  ));
}

logp("echo","Milling files locator script complete.");

// close log
logp_close();

exit();

?>
