EVSE_D = ./evse
OCPP_D = ./ocpp

EVSE_T = evse
OCPP_T = ocpp

.PHONY: all
all: clean build 

build: 
	-cd $(EVSE_D) && $(MAKE) $(EVSE_T)
	-cd $(OCPP_D) && $(MAKE) $(OCPP_T)


.PHONY: clean
clean:
	-cd $(EVSE_D) && $(MAKE) clean
	-cd $(OCPP_D) && $(MAKE) clean
