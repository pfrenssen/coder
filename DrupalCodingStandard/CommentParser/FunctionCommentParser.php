<?php
/**
 * Parses function doc comments.
 *
 * PHP version 5
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @author   Klaus Purer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */

/**
 * Parses function doc comments.
 *
 * @category PHP
 * @package  PHP_CodeSniffer
 * @author   Klaus Purer
 * @link     http://pear.php.net/package/PHP_CodeSniffer
 */
class DrupalCodingStandard_CommentParser_FunctionCommentParser extends PHP_CodeSniffer_CommentParser_FunctionCommentParser
{

    /**
     * The parameter elements within this function comment.
     *
     * @var array(DrupalCodingStandard_CommentParser_ParameterElement)
     */
    protected $params = array();


    /**
     * Parses parameter elements.
     *
     * @param array(string) $tokens The tokens that conmpise this sub element.
     *
     * @return DrupalCodingStandard_CommentParser_ParameterElement
     */
    protected function parseParam($tokens)
    {
        $param = new DrupalCodingStandard_CommentParser_ParameterElement(
            $this->previousElement,
            $tokens,
            $this->phpcsFile
        );

        $this->params[] = $param;
        return $param;

    }//end parseParam()


    /**
     * Returns the parameter elements that this function comment contains.
     *
     * Returns an empty array if no parameter elements are contained within
     * this function comment.
     *
     * @return array(DrupalCodingStandard_CommentParser_ParameterElement)
     */
    public function getParams()
    {
        return $this->params;

    }//end getParams()


}//end class

?>
