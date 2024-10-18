.. WaterPaths documentation master file, created by
   sphinx-quickstart on Thu Oct 17 21:12:52 2024.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. toctree::
   :maxdepth: 4
   :caption: Contents:

Welcome to WaterPaths's documentation!
======================================

.. doxygenfile:: ../src/main.cpp
.. doxygenfile:: ../src/ContinuityModels/ContinuityModelROF.cpp
.. doxygenfile:: ../src/ContinuityModels/ContinuityModelROF.h
.. doxygenfile:: ../src/ContinuityModels/ContinuityModelRealization.cpp
.. doxygenfile:: ../src/ContinuityModels/ContinuityModelRealization.h
.. doxygenfile:: ../src/ContinuityModels/Base/ContinuityModel.cpp
.. doxygenfile:: ../src/ContinuityModels/Base/ContinuityModel.h

.. doxygenfile:: ../src/Controls/EvaporationSeries.cpp
.. doxygenfile:: ../src/Controls/EvaporationSeries.h
.. doxygenfile:: ../src/Controls/FixedMinEnvFlowControl.cpp
.. doxygenfile:: ../src/Controls/FixedMinEnvFlowControl.h
.. doxygenfile:: ../src/Controls/InflowMinEnvFlowControl.cpp
.. doxygenfile:: ../src/Controls/InflowMinEnvFlowControl.h
.. doxygenfile:: ../src/Controls/SeasonalMinEnvFlowControl.cpp
.. doxygenfile:: ../src/Controls/SeasonalMinEnvFlowControl.h
.. doxygenfile:: ../src/Controls/StorageMinEnvFlowControl.cpp
.. doxygenfile:: ../src/Controls/StorageMinEnvFlowControl.h
.. doxygenfile:: ../src/Controls/WwtpDischargeRule.cpp
.. doxygenfile:: ../src/Controls/WwtpDischargeRule.h
.. doxygenfile:: ../src/Controls/Base/ControlRules.h
.. doxygenfile:: ../src/Controls/Base/MinEnvFlowControl.cpp
.. doxygenfile:: ../src/Controls/Base/MinEnvFlowControl.h
.. doxygenfile:: ../src/Controls/Base/MinEnvironFlowControl.cpp
.. doxygenfile:: ../src/Controls/Base/MinEnvironFlowControl.h
.. doxygenfile:: ../src/Controls/Custom/FallsLakeMinEnvFlowControl.cpp
.. doxygenfile:: ../src/Controls/Custom/FallsLakeMinEnvFlowControl.h
.. doxygenfile:: ../src/Controls/Custom/JordanLakeMinEnvFlowControl.cpp
.. doxygenfile:: ../src/Controls/Custom/JordanLakeMinEnvFlowControl.h

.. doxygenfile:: ../src/DataCollector/AllocatedReservoirDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/AllocatedReservoirDataCollector.h
.. doxygenfile:: ../src/DataCollector/EmptyDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/EmptyDataCollector.h
.. doxygenfile:: ../src/DataCollector/IntakeDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/IntakeDataCollector.h
.. doxygenfile:: ../src/DataCollector/MasterDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/MasterDataCollector.h
.. doxygenfile:: ../src/DataCollector/QuaryDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/QuaryDataCollector.h
.. doxygenfile:: ../src/DataCollector/ReservoirDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/ReservoirDataCollector.h
.. doxygenfile:: ../src/DataCollector/RestrictionsDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/RestrictionsDataCollector.h
.. doxygenfile:: ../src/DataCollector/StreamDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/StreamDataCollector.h
.. doxygenfile:: ../src/DataCollector/UtilitiesDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/UtilitiesDataCollector.h
.. doxygenfile:: ../src/DataCollector/WaterReuseDataCollector.cpp
.. doxygenfile:: ../src/DataCollector/WaterReuseDataCollector.h
.. doxygenfile:: ../src/DataCollector/Base/DataCollector.cpp
.. doxygenfile:: ../src/DataCollector/Base/DataCollector.h

.. doxygenfile:: ../src/DroughtMitigationInstruments/InsuranceStorageToROF.cpp
.. doxygenfile:: ../src/DroughtMitigationInstruments/InsuranceStorageToROF.h
.. doxygenfile:: ../src/DroughtMitigationInstruments/Restrictions.cpp
.. doxygenfile:: ../src/DroughtMitigationInstruments/Restrictions.h
.. doxygenfile:: ../src/DroughtMitigationInstruments/Transfers.cpp
.. doxygenfile:: ../src/DroughtMitigationInstruments/Transfers.h
.. doxygenfile:: ../src/DroughtMitigationInstruments/Base/DroughtMitigationPolicy.cpp
.. doxygenfile:: ../src/DroughtMitigationInstruments/Base/DroughtMitigationPolicy.h

.. doxygenfile:: ../src/Problem/PaperTestProblem.cpp
.. doxygenfile:: ../src/Problem/PaperTestProblem.h
.. doxygenfile:: ../src/Problem/Triangle.cpp
.. doxygenfile:: ../src/Problem/Triangle.h
.. doxygenfile:: ../src/Problem/Base/Problem.cpp
.. doxygenfile:: ../src/Problem/Base/Problem.h

.. doxygenfile:: ../src/Simulation/Simulation.cpp  
.. doxygenfile:: ../src/Simulation/Simulation.h

.. doxygenfile:: ../src/SystemComponents/Catchment.cpp
.. doxygenfile:: ../src/SystemComponents/Catchment.h
.. doxygenfile:: ../src/SystemComponents/Bonds/BalloonPaymentBond.cpp
.. doxygenfile:: ../src/SystemComponents/Bonds/BalloonPaymentBond.h
.. doxygenfile:: ../src/SystemComponents/Bonds/FloatingInterestBalloonPaymentBond.cpp
.. doxygenfile:: ../src/SystemComponents/Bonds/FloatingInterestBalloonPaymentBond.h
.. doxygenfile:: ../src/SystemComponents/Bonds/LevelDebtServiceBond.cpp
.. doxygenfile:: ../src/SystemComponents/Bonds/LevelDebtServiceBond.h
.. doxygenfile:: ../src/SystemComponents/Bonds/Base/Bond.cpp
.. doxygenfile:: ../src/SystemComponents/Bonds/Base/Bond.h
.. doxygenfile:: ../src/SystemComponents/Utility/InfrastructureManager.cpp
.. doxygenfile:: ../src/SystemComponents/Utility/InfrastructureManager.h
.. doxygenfile:: ../src/SystemComponents/Utility/Utility.cpp
.. doxygenfile:: ../src/SystemComponents/Utility/Utility.h
.. doxygenfile:: ../src/SystemComponents/WaterSources/AllocatedReservoir.cpp
.. doxygenfile:: ../src/SystemComponents/WaterSources/AllocatedReservoir.h
.. doxygenfile:: ../src/SystemComponents/WaterSources/Intake.cpp
.. doxygenfile:: ../src/SystemComponents/WaterSources/Intake.h
.. doxygenfile:: ../src/SystemComponents/WaterSources/Quarry.cpp
.. doxygenfile:: ../src/SystemComponents/WaterSources/Quarry.h
.. doxygenfile:: ../src/SystemComponents/WaterSources/Relocation.cpp
.. doxygenfile:: ../src/SystemComponents/WaterSources/Relocation.h
.. doxygenfile:: ../src/SystemComponents/WaterSources/Reservoir.cpp
.. doxygenfile:: ../src/SystemComponents/WaterSources/Reservoir.h
.. doxygenfile:: ../src/SystemComponents/WaterSources/ReservoirExpansion.cpp
.. doxygenfile:: ../src/SystemComponents/WaterSources/ReservoirExpansion.h
.. doxygenfile:: ../src/SystemComponents/WaterSources/SequentialJointTreatmentExpansion.cpp
.. doxygenfile:: ../src/SystemComponents/WaterSources/SequentialJointTreatmentExpansion.h
.. doxygenfile:: ../src/SystemComponents/WaterSources/WaterReuse.cpp
.. doxygenfile:: ../src/SystemComponents/WaterSources/WaterReuse.h
.. doxygenfile:: ../src/SystemComponents/WaterSources/Base/WaterSource.cpp
.. doxygenfile:: ../src/SystemComponents/WaterSources/Base/WaterSource.h

.. doxygenfile:: ../src/Utils/Constants.h
.. doxygenfile:: ../src/Utils/DataSeries.cpp
.. doxygenfile:: ../src/Utils/DataSeries.h
.. doxygenfile:: ../src/Utils/DataSeries.h
.. doxygenfile:: ../src/Utils/Exceptions.cpp
.. doxygenfile:: ../src/Utils/Matrices.h
.. doxygenfile:: ../src/Utils/ObjectivesCalculator.cpp
.. doxygenfile:: ../src/Utils/ObjectivesCalculator.h
.. doxygenfile:: ../src/Utils/Solutions.h
.. doxygenfile:: ../src/Utils/Utils.cpp
.. doxygenfile:: ../src/Utils/Utils.h
.. doxygenfile:: ../src/Utils/Graph/Graph.cpp
.. doxygenfile:: ../src/Utils/Graph/Graph.h
.. doxygenfile:: ../src/Utils/QPSolver/Array.cpp
.. doxygenfile:: ../src/Utils/QPSolver/Array.h
.. doxygenfile:: ../src/Utils/QPSolver/QuadProg++.cpp
.. doxygenfile:: ../src/Utils/QPSolver/QuadProg++.h

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
