<?php

/**
 * Provides a 'Delete any path alias' action.
 *
 * @todo Add access callback information from Drupal 7.
 * @todo Add group information from Drupal 7.
 *
 * @Action(
 *   id = "rules_path_alias_delete",
 *   label = @Translation("Delete any path alias"),
 *   context = {
 *     "alias" = @ContextDefinition("string",
 *       label = @Translation("Existing system path alias"),
 *       description = @Translation("Specifies the existing path alias you wish to delete, for example 'about/team'. Use a relative path and do not add a trailing slash.")
 *     )
 *   }
 * )
 */
class GoodAnnotation extends RulesActionBase implements ContainerFactoryPluginInterface {}
