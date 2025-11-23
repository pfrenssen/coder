<?php

/**
 * Another test.
 */
class GoodReferenceDocs {

  /**
   * Parameters described by reference are OK.
   *
   * @param array &$form
   *   The form array.
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   The form state.
   *
   * @return array
   *   A renderable array.
   */
  public function removeQueueItem(array &$form, FormStateInterface $form_state) {
    $trigger = $form_state->getTriggeringElement();
    $i = $trigger['#parents'][1];

    $queues = $form_state->getValue('watch_queues', []);
    $queues[$i]['to_remove'] = 1;
    $form_state->setValue('watch_queues', $queues);
    $this->rebuild($form_state, $form);

    drupal_set_message($this->t('Item will be removed permanently when configuration is saved.'));
    return StatusMessages::renderMessages(NULL);
  }

  /**
   * Parameters described by reference are OK.
   *
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   The form state.
   * @param array &$old_form
   *   The old form build.
   *
   * @return array
   *   The newly built form.
   */
  protected function rebuild(FormStateInterface $form_state, array &$old_form) {
    $form_state->setRebuild();
    $form = $this->formBuilder
      ->rebuildForm($this->getFormId(), $form_state, $old_form);
    return $form;
  }

}
