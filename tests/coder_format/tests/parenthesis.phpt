<?php
TEST: Parenthesis

--INPUT--
// No change:
function drupal_mail_send($message) {
  return mail(
    $message['to'],
    mime_header_encode($message['subject']),
    // Note: e-mail uses CRLF for line-endings, but PHP's API requires LF.
    // They will appear correctly in the actual e-mail that is sent.
    str_replace("\r", '', $message['body']),
    join("\n", $mimeheaders)
  );
}


--EXPECT--
// No change:
function drupal_mail_send($message) {
  return mail(
    $message['to'],
    mime_header_encode($message['subject']),
    // Note: e-mail uses CRLF for line-endings, but PHP's API requires LF.
    // They will appear correctly in the actual e-mail that is sent.
    str_replace("\r", '', $message['body']),
    join("\n", $mimeheaders)
  );
}

