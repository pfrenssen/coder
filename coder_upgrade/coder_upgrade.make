; Drush Make (http://drupal.org/project/drush_make)
api = 2

; Drupal core

core = 7.x
projects[drupal] = 7

; Dependencies

; use this for development
;projects[grammar_parser_lib][type] = module
;projects[grammar_parser_lib][download][type] = git
;projects[grammar_parser_lib][download][url] = http://git.drupal.org/project/grammar_parser_lib.git
;projects[grammar_parser_lib][download][branch] = 7.x-1.x

; use this for production
projects[grammar_parser_lib] = 1
