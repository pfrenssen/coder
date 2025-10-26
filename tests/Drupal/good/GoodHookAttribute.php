<?php

/**
 * Implements hook_cron().
 */
#[Hook('cron')]
class GoodHookAttribute {

  public function __construct(
    private readonly EntityTypeManagerInterface $entityTypeManager,
    private readonly StreamWrapperManagerInterface $streamWrapperManager,
    private readonly ConfigFactoryInterface $configFactory,
    private readonly FileUsageInterface $fileUsage,
    private readonly TimeInterface $time,
    #[Autowire('@logger.channel.file')]
    private readonly LoggerInterface $logger,
  ) {}

}
