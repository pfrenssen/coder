<?php

class DrupalPractice_Project {

    /**
     * Bla.
     * 
     * @param PHP_CodeSniffer_File $phpcsFile The file being scanned.
     */
    public static function getName(PHP_CodeSniffer_File $phpcsFile) {
        // Cache the project name per file as this might get called often.
        static $cache;

        if (isset($cache[$phpcsFile->getFilename()])) {
            return $cache[$phpcsFile->getFilename()];
        }

        $path_parts = pathinfo($phpcsFile->getFilename());
        // Module and install files are easy: they contain the project name in the
        // file name.
        if (isset($path_parts['extension']) && ($path_parts['extension'] === 'module' || $path_parts['extension'] === 'install')) {
            $cache[$phpcsFile->getFilename()] = $path_parts['filename'];
            return $path_parts['filename'];
        }

        // Search for an info file.
        $dir = $path_parts['dirname'];
        do {
          $infoFiles = glob("$dir/*.info");
          // Go one directory up if we do not find an info file here.
          $dir = dirname($dir);
        }
        while (empty($infoFiles) && $dir != dirname($dir));

        // No info file found, so we give up.
        if (empty($infoFiles)) {
            $cache[$phpcsFile->getFilename()] = false;
            return false;
        }

        // Sort the info file names and take the shortest info file.
        usort($infoFiles, array('DrupalPractice_Project', 'compareLength'));
        $infoFile = $infoFiles[0];
        $path_parts = pathinfo($infoFile);
        $cache[$phpcsFile->getFilename()] = $path_parts['filename'];
        return $path_parts['filename'];
    }

    public static function compareLength($a, $b) {
        return strlen($a) - strlen($b);
    }
}
