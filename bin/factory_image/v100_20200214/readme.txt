Combined into userprog.mot by using:
	bootloader part:
		\bin\boot_loader\v100_20200211\rx72n_boot_loader.mot
	bank0 part:
		\bin\user_application\segger_emwin_demos\v100_20200214\segger_emwin_demos.mot
	bank1 part:
		\bin\user_application\aws_demos\v093_20200214\aws_demos.mot
	bootloader key pair:
		\sample_keys\secp256r1.publickey

Signed bank0/bank1 binary and input these signature into userprog.mot by using:
		\sample_keys\secp256r1.privatekey

	