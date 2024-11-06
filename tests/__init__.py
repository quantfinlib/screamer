from devtools import load_screamer_module, logger
screamer_module = load_screamer_module()
logger.info('in test, loaded the screamer_module')
logger.info(dir(screamer_module))

