-- MCU Command Control (MCUCC) Component
-- Robotics Club at UCF
-- Jonathan Mohlenhoff - jmohlenh@ist.ucf.edu

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

use work.MCUPRT.all;
use work.SevenSegment.all;
use work.AnalogSample.all;
use work.sampleFrequency.all;
use work.channel.all;
use work.MAX268.all;
use work.arbiter.all;
use work.dPotController.all;
use work.RawData.all;
use work.PingTimer.all;

entity mcuccComp is
	Port
	(
			clock        : in std_logic;
			
			TXD1          : out std_logic;
			RXD1          : in std_logic;
			
			TXD2          : out std_logic;
			RXD2          : in std_logic;
			
			IN0			  : in std_logic;
			IN1           : in std_logic;
			IN2			  : in std_logic;
			IN3			  : in std_logic;
			
			SDA0			 : inout std_logic;
			SCL0			 : inout std_logic;
			
			SDA1			 : inout std_logic;
			SCL1			 : inout std_logic;
			
			sData			 : in std_logic_vector(5 downto 0);
			sClock 		 : out std_logic_vector(2 downto 0);
			nCS 			 : out std_logic_vector(2 downto 0);
			
			d_out        : out std_logic_vector(7 downto 0) := x"00";
			d_in			 : in std_logic_vector(7 downto 0);
			
			pwmOut		 : out std_logic;
			FN			    : buffer std_logic_vector(4 downto 0) := "00000";
			Q				 : out std_logic_vector(6 downto 0) := "1000000";
			
			sevenSegC	 : out std_logic_vector(6 downto 0);
			sevenSegA	 : out std_logic_vector(3 downto 0)		
	);
end mcuccComp;

architecture Behavioral of mcuccComp is
	
	signal rt_commandRx : std_logic_vector (7 downto 0);
	signal rt_dataRx : std_logic_vector (31 downto 0);
	signal rt_packetRx : std_logic;
	signal rt_packetRd : std_logic;
	
	signal rt_commandTx : std_logic_vector (7 downto 0);
	signal rt_dataTx : std_logic_vector (31 downto 0);
	signal rt_packetTx : std_logic;
	signal rt_packetSent : std_logic; -- buffer is empty
	
	type commandState is (waitForCommand, 
								 processCommand,
								 sendFFT,
								 waitFFT,
								 delayFFT);
								 
	signal commandStateCurrent : commandState := waitForCommand;
	signal commandStateNext : commandState;
	
	signal max268Interface_freqValueIn : std_logic_vector(15 downto 0);
	
	signal digit0, digit1, digit2, digit3 : std_logic_vector(3 downto 0);
	signal max268DataOut : std_logic_vector(15 downto 0);
	
	signal analog0 : std_logic_vector (11 downto 0);
	signal analog1 : std_logic_vector (11 downto 0);
	signal analog2 : std_logic_vector (11 downto 0);
	signal analog3 : std_logic_vector (11 downto 0);
	signal analog4 : std_logic_vector (11 downto 0);
	signal analog5 : std_logic_vector (11 downto 0);
	signal analogDone : std_logic;
	
	--Channel0
	signal dcOffset0 : std_logic_vector (11 downto 0);
	signal max0 : std_logic_vector (11 downto 0);
	signal min0 : std_logic_vector (11 downto 0);
	signal pingOut0 : std_logic;
	signal AdderOut0 : std_logic_vector (7 downto 0);
	signal AdderDone0 : std_logic;
	signal vgnA0 : std_logic_vector (7 downto 0);
	signal vgnB0 : std_logic_vector (7 downto 0);
	signal vref0 : std_logic_vector (7 downto 0);
	
	--Channel1
	signal dcOffset1 : std_logic_vector (11 downto 0);
	signal max1 : std_logic_vector (11 downto 0);
	signal min1 : std_logic_vector (11 downto 0);
	signal pingOut1 : std_logic;
	signal AdderOut1 : std_logic_vector (7 downto 0);
	signal AdderDone1 : std_logic;
	signal vgnA1 : std_logic_vector (7 downto 0);
	signal vgnB1 : std_logic_vector (7 downto 0);
	signal vref1 : std_logic_vector (7 downto 0);
	
	
	--Channel2
	signal dcOffset2 : std_logic_vector (11 downto 0);
	signal max2 : std_logic_vector (11 downto 0);
	signal min2 : std_logic_vector (11 downto 0);
	signal pingOut2 : std_logic;
	signal AdderOut2 : std_logic_vector (7 downto 0);
	signal AdderDone2 : std_logic;
	signal vgnA2 : std_logic_vector (7 downto 0);
	signal vgnB2 : std_logic_vector (7 downto 0);
	signal vref2 : std_logic_vector (7 downto 0);
	

	--Channel3
	signal dcOffset3 : std_logic_vector (11 downto 0);
	signal max3 : std_logic_vector (11 downto 0);
	signal min3 : std_logic_vector (11 downto 0);
	signal pingOut3 : std_logic;
	signal AdderOut3 : std_logic_vector (7 downto 0);
	signal AdderDone3 : std_logic;
	signal vgnA3 : std_logic_vector (7 downto 0);
	signal vgnB3 : std_logic_vector (7 downto 0);
	signal vref3 : std_logic_vector (7 downto 0);
	

	--Channel4
	signal dcOffset4 : std_logic_vector (11 downto 0);
	signal max4 : std_logic_vector (11 downto 0);
	signal min4 : std_logic_vector (11 downto 0);
	signal pingOut4 : std_logic;
	signal AdderOut4 : std_logic_vector (7 downto 0);
	signal AdderDone4 : std_logic;
	signal vgnA4 : std_logic_vector (7 downto 0);
	signal vgnB4 : std_logic_vector (7 downto 0);
	signal vref4 : std_logic_vector (7 downto 0);
	
	
	signal dcOffsetIncrement : std_logic_vector (23 downto 0) := x"000080";
	signal pkToPkDecayValue : std_logic_vector (23 downto 0) := x"000080";	
	signal pingDetectSilentTime : std_logic_vector (11 downto 0) := x"0FF";
	signal pingDetectPingCount : std_logic_vector (11 downto 0) := x"0FF";
	
	signal pingDetectMinPingThreshold : std_logic_vector (11 downto 0) := x"300";
	signal pingDetectAbsoluteFlag : std_logic := '0';
	signal pingDetectThresholdRelative : std_logic_vector (2 downto 0) := "010";
	signal pingDetectThresholdAbsMax : std_logic_vector (11 downto 0) := x"000";
	signal pingDetectThresholdAbsMin : std_logic_vector (11 downto 0) := x"000";
	signal pingDetectThresholdAbsFromDC : std_logic_vector (11 downto 0) := x"000";
	
	signal sampleFrequency : std_logic_vector (31 downto 0);
	signal samplePeriod : std_logic_vector (11 downto 0);
	
	signal pingTime0 : std_logic_vector (31 downto 0) := x"00000000";
	signal pingTime1 : std_logic_vector (31 downto 0) := x"00000000";
	signal pingTime2 : std_logic_vector (31 downto 0) := x"00000000";
	signal pingTime3 : std_logic_vector (31 downto 0) := x"00000000";
	signal pingTime4 : std_logic_vector (31 downto 0) := x"00000000";
	
	signal rawMode : std_logic_vector (1 downto 0) := "00";
	signal rawDataMode : std_logic := '0';
	
	signal fft_max_index : std_logic_vector (23 downto 0);
	signal fft0_re : std_logic_vector (23 downto 0);
	signal fft0_im : std_logic_vector (23 downto 0);
	signal fft1_re : std_logic_vector (23 downto 0);
	signal fft1_im : std_logic_vector (23 downto 0);
	signal fft2_re : std_logic_vector (23 downto 0);
	signal fft2_im : std_logic_vector (23 downto 0);
	signal fft3_re : std_logic_vector (23 downto 0);
	signal fft3_im : std_logic_vector (23 downto 0);
	signal fft4_re : std_logic_vector (23 downto 0);
	signal fft4_im : std_logic_vector (23 downto 0);
	signal fft_ready : std_logic;
	signal fft_read : std_logic;
	
	signal arbiterMode : std_logic_vector (1 downto 0) := "00";

	signal vgnM : std_logic_vector (7 downto 0);
	
begin

	mcuprtInterface: mcuprtComp port map
	(
		TXD => TXD1,
		RXD => RXD1,
		clock => clock,
		commandRx => rt_commandRx,
		dataRx => rt_dataRx,
		packetRx => rt_packetRx,
		packetRd => rt_packetRd,
		commandTx => rt_commandTx,
		dataTx => rt_dataTx,
		packetTx => rt_packetTx,
		packetSent => rt_packetSent
	);
	
	
	channel0 : channelComp port map
	(
		clock => clock,
		
		analogIn => analog0,
		analogDone => analogDone,
		
		dcCalc_incrementValue => dcOffsetIncrement,
		dcCalc_dc => dcOffset0,
		
		pkToPk_decayValue => pkToPkDecayValue,
		pkToPk_maxValue => max0,
		pkToPk_minValue => min0,
		
		pingDetect_silentTime => pingDetectSilentTime,
		pingDetect_pingCount => pingDetectPingCount,
		
		pingDetect_minPingThreshold => pingDetectMinPingThreshold,
		pingDetect_absoluteFlag => pingDetectAbsoluteFlag,	
		pingDetect_thresholdRelative => pingDetectThresholdRelative,
		pingDetect_thresholdAbsMax => pingDetectThresholdAbsMax,
		pingDetect_thresholdAbsMin => pingDetectThresholdAbsMin,
		pingDetect_thresholdAbsFromDC => pingDetectThresholdAbsFromDC,
		
		pingDetect_pingOut => pingOut0,
		
		adder_value => AdderOut0,
		adder_done => AdderDone0
	);
	
	channel1 : channelComp port map
	(
		clock => clock,
		
		analogIn => analog1,
		analogDone => analogDone,
		
		dcCalc_incrementValue => dcOffsetIncrement,
		dcCalc_dc => dcOffset1,
		
		pkToPk_decayValue => pkToPkDecayValue,
		pkToPk_maxValue => max1,
		pkToPk_minValue => min1,
		
		pingDetect_silentTime => pingDetectSilentTime,
		pingDetect_pingCount => pingDetectPingCount,
		
		pingDetect_minPingThreshold => pingDetectMinPingThreshold,
		pingDetect_absoluteFlag => pingDetectAbsoluteFlag,	
		pingDetect_thresholdRelative => pingDetectThresholdRelative,
		pingDetect_thresholdAbsMax => pingDetectThresholdAbsMax,
		pingDetect_thresholdAbsMin => pingDetectThresholdAbsMin,
		pingDetect_thresholdAbsFromDC => pingDetectThresholdAbsFromDC,
		
		pingDetect_pingOut => pingOut1,
		
		adder_value => AdderOut1,
		adder_done => AdderDone1
	);			
	
	channel2 : channelComp port map
	(
		clock => clock,
		
		analogIn => analog2,
		analogDone => analogDone,
		
		dcCalc_incrementValue => dcOffsetIncrement,
		dcCalc_dc => dcOffset2,
		
		pkToPk_decayValue => pkToPkDecayValue,
		pkToPk_maxValue => max2,
		pkToPk_minValue => min2,
		
		pingDetect_silentTime => pingDetectSilentTime,
		pingDetect_pingCount => pingDetectPingCount,
		
		pingDetect_minPingThreshold => pingDetectMinPingThreshold,
		pingDetect_absoluteFlag => pingDetectAbsoluteFlag,	
		pingDetect_thresholdRelative => pingDetectThresholdRelative,
		pingDetect_thresholdAbsMax => pingDetectThresholdAbsMax,
		pingDetect_thresholdAbsMin => pingDetectThresholdAbsMin,
		pingDetect_thresholdAbsFromDC => pingDetectThresholdAbsFromDC,
		
		pingDetect_pingOut => pingOut2,
		
		adder_value => AdderOut2,
		adder_done => AdderDone2
	);			
	
	channel3 : channelComp port map
	(
		clock => clock,
		
		analogIn => analog3,
		analogDone => analogDone,
		
		dcCalc_incrementValue => dcOffsetIncrement,
		dcCalc_dc => dcOffset3,
		
		pkToPk_decayValue => pkToPkDecayValue,
		pkToPk_maxValue => max3,
		pkToPk_minValue => min3,
		
		pingDetect_silentTime => pingDetectSilentTime,
		pingDetect_pingCount => pingDetectPingCount,
		
		pingDetect_minPingThreshold => pingDetectMinPingThreshold,
		pingDetect_absoluteFlag => pingDetectAbsoluteFlag,	
		pingDetect_thresholdRelative => pingDetectThresholdRelative,
		pingDetect_thresholdAbsMax => pingDetectThresholdAbsMax,
		pingDetect_thresholdAbsMin => pingDetectThresholdAbsMin,
		pingDetect_thresholdAbsFromDC => pingDetectThresholdAbsFromDC,
		
		pingDetect_pingOut => pingOut3,
		
		adder_value => AdderOut3,
		adder_done => AdderDone3
	);			
	
	channel4 : channelComp port map
	(
		clock => clock,
		
		analogIn => analog4,
		analogDone => analogDone,
		
		dcCalc_incrementValue => dcOffsetIncrement,
		dcCalc_dc => dcOffset4,
		
		pkToPk_decayValue => pkToPkDecayValue,
		pkToPk_maxValue => max4,
		pkToPk_minValue => min4,
		
		pingDetect_silentTime => pingDetectSilentTime,
		pingDetect_pingCount => pingDetectPingCount,
		
		pingDetect_minPingThreshold => pingDetectMinPingThreshold,
		pingDetect_absoluteFlag => pingDetectAbsoluteFlag,
		pingDetect_thresholdRelative => pingDetectThresholdRelative,
		pingDetect_thresholdAbsMax => pingDetectThresholdAbsMax,
		pingDetect_thresholdAbsMin => pingDetectThresholdAbsMin,
		pingDetect_thresholdAbsFromDC => pingDetectThresholdAbsFromDC,
		
		pingDetect_pingOut => pingOut4,
		
		adder_value => AdderOut4,
		adder_done => AdderDone4
	);			
	
	
	max268Interface: max268Comp port map
	(
		clk => clock,
		freqValueIn => max268Interface_freqValueIn,
		pwmOut => pwmOut,
		FN => FN
	);
	
	sevenSegmentInterface: SegmentRefComp port map
	(
		CLK => clock,
		
		-- Four user inputs
		input1 => digit0,
		input2 => digit1,
		input3 => digit2,
		input4 => digit3,
		
		--Actual seven segment display and anode 
		sevenseg1 => sevenSegC,
		anodes => sevenSegA
	);
	
	analogInterface: AnalogSampleComp port map
	(
		clk => clock,		
		sData => sData,
		sClock => sClock,
		nCS => nCS,	
		analog0 => analog1,
		analog1 => analog0,
		analog2 => analog3,
		analog3 => analog2,
		analog4 => analog4,
		analog5 => analog5,
		done => analogDone
	);
	
	sampleFreqInterface: sampleFrequencyComp port map
	(
		clk => clock,
		AdDone => analogDone,
		AdFrequency => sampleFrequency,
		AdPeriod => samplePeriod
	);
	
	arbiterInterface: arbiterComp port map
	(
		clock => clock,
		
		dcCalc_dc0 => dcOffset0,
		pkToPk_maxValue0 => max0,
		pkToPk_minValue0 => min0,
		pingDetect_pingOut0 => pingOut0,
		vgnA0 => vgnA0,
		vgnB0 => vgnB0,
		vref0 => vref0,
		
		dcCalc_dc1 => dcOffset1,
		pkToPk_maxValue1 => max1,
		pkToPk_minValue1 => min1,
		pingDetect_pingOut1 => pingOut1,
		vgnA1 => vgnA1,
		vgnB1 => vgnB1,
		vref1 => vref1,

		dcCalc_dc2 => dcOffset2,
		pkToPk_maxValue2 => max2,
		pkToPk_minValue2 => min2,
		pingDetect_pingOut2 => pingOut2,
		vgnA2 => vgnA2,
		vgnB2 => vgnB2,
		vref2 => vref2,

		dcCalc_dc3 => dcOffset3,
		pkToPk_maxValue3 => max3,
		pkToPk_minValue3 => min3,
		pingDetect_pingOut3 => pingOut3,
		vgnA3 => vgnA3,
		vgnB3 => vgnB3,
		vref3 => vref3,

		dcCalc_dc4 => dcOffset4,
		pkToPk_maxValue4 => max4,
		pkToPk_minValue4 => min4,
		pingDetect_pingOut4 => pingOut4,
		vgnA4 => vgnA4,
		vgnB4 => vgnB4,
		vref4 => vref4,
		
		mode => arbiterMode,
		vgnM => vgnM
	);
	
	dPotControllerInterface: dPotControllerComp port map
	(
		clock => clock,
		
		SCL0 => SCL0, 
		SDA0 => SDA0, 
		
		SCL1 => SCL1,
		SDA1 => SDA1,
		
		adderOut0 => AdderOut0,
		vgnA0 => vgnA0,
		vgnB0 => vgnB0,
		vref0 => vref0,
		
		adderOut1 => AdderOut1,
		vgnA1 => vgnA1,
		vgnB1 => vgnB1,
		vref1 => vref1,
		
		adderOut2 => AdderOut2,
		vgnA2 => vgnA2,
		vgnB2 => vgnB2,
		vref2 => vref2,
		
		adderOut3 => AdderOut3,
		vgnA3 => vgnA3,
		vgnB3 => vgnB3,
		vref3 => vref3,
		
		adderOut4 => AdderOut4,
		vgnA4 => vgnA4,
		vgnB4 => vgnB4,
		vref4 => vref4
	);
	
	rawDataInterface: rawDataComp port map
	(
		TXD => TXD2,
		RXD => RXD2,
		clock	=> clock,
		mode => rawMode,
		dataMode => rawDataMode,
		analogDone => analogDone,
		analog0 => analog0,
		analog1 => analog1,
		analog2 => analog2,
		analog3 => analog3,
		analog4 => analog4,
		ping0	=> pingOut0,
		ping1	=> pingOut1,
		ping2	=> pingOut2,
		ping3	=> pingOut3,
		ping4	=> pingOut4,
		fft_index => fft_max_index,
		fft0_re => fft0_re,
		fft0_im => fft0_im,
		fft1_re => fft1_re,
		fft1_im => fft1_im,
		fft2_re => fft2_re,
		fft2_im => fft2_im,
		fft3_re => fft3_re,
		fft3_im => fft3_im,
		fft4_re => fft4_re,
		fft4_im => fft4_im,
		fft_ready => fft_ready,
		fft_read => fft_read
	);
	
	pingTimerInterface: pingTimerComp port map
	(
		clock	=> clock,
		ping0 => pingOut0,
		ping1 => pingOut1,
		ping2 => pingOut2,
		ping3 => pingOut3,
		ping4 => pingOut4,
		pingTime0 => pingTime0,
		pingTime1 => pingTime1,
		pingTime2 => pingTime2,
		pingTime3 => pingTime3,
		pingTime4 => pingTime4
	);

--d_out(0) <= pingOut0;
--d_out(1) <= pingOut1;
--d_out(2) <= pingOut2;
--d_out(3) <= pingOut3;
--d_out(4) <= pingOut0 and pingOut1 and pingOut2 and pingOut3;

--d_out(7) <= analogDone;

--digit0 <= "00"&rawMode;
--digit1 <= x"0";
--digit2 <= x"0";
--digit3 <= x"0";

--process(clock)
--	begin
--		if (rising_edge(clock)) then
--			if (d_in(1) = '0' and d_in(0) = '0') then
--				if (d_in(7) = '0') then
--					digit3 <= pingTime0(31 downto 28);
--					digit2 <= pingTime0(27 downto 24);
--					digit1 <= pingTime0(23 downto 20);
--					digit0 <= pingTime0(19 downto 16);
--				else
--					digit3 <= pingTime0(15 downto 12);
--					digit2 <= pingTime0(11 downto 8);
--					digit1 <= pingTime0(7 downto 4);
--					digit0 <= pingTime0(3 downto 0);
--				end if;
--			elsif (d_in(1) = '0' and d_in(0) = '1') then
--				if (d_in(7) = '0') then
--					digit3 <= pingTime1(31 downto 28);
--					digit2 <= pingTime1(27 downto 24);
--					digit1 <= pingTime1(23 downto 20);
--					digit0 <= pingTime1(19 downto 16);
--				else
--					digit3 <= pingTime1(15 downto 12);
--					digit2 <= pingTime1(11 downto 8);
--					digit1 <= pingTime1(7 downto 4);
--					digit0 <= pingTime1(3 downto 0);
--				end if;
--			elsif (d_in(1) = '1' and d_in(0) = '0') then
--				if (d_in(7) = '0') then
--					digit3 <= pingTime2(31 downto 28);
--					digit2 <= pingTime2(27 downto 24);
--					digit1 <= pingTime2(23 downto 20);
--					digit0 <= pingTime2(19 downto 16);
--				else
--					digit3 <= pingTime2(15 downto 12);
--					digit2 <= pingTime2(11 downto 8);
--					digit1 <= pingTime2(7 downto 4);
--					digit0 <= pingTime2(3 downto 0);
--				end if;
--			elsif (d_in(1) = '1' and d_in(0) = '1') then
--				if (d_in(7) = '0') then
--					digit3 <= pingTime3(31 downto 28);
--					digit2 <= pingTime3(27 downto 24);
--					digit1 <= pingTime3(23 downto 20);
--					digit0 <= pingTime3(19 downto 16);
--				else
--					digit3 <= pingTime3(15 downto 12);
--					digit2 <= pingTime3(11 downto 8);
--					digit1 <= pingTime3(7 downto 4);
--					digit0 <= pingTime3(3 downto 0);
--				end if;
--			end if;
--		end if;
--	end process;

process(clock)
	begin
		if (rising_edge(clock)) then
			commandStateCurrent <= commandStateNext;
		end if;
	end process;

process(clock, rt_commandRx)
	
	variable tempD_outPort, tempD_outValue: std_logic_vector (7 downto 0);
	variable digitalPotChannel, digitalPotValue : std_logic_vector (7 downto 0); 
	
	type array1 is array (0 to 11) of std_logic_vector (23 downto 0);
	variable fftV : array1;
	
	variable fftCount : unsigned(3 downto 0);
	variable fftDelayCounter : integer := 0;
	variable selectIndex : integer := 0;
	
	begin
		if (rising_edge(clock)) then
			selectIndex := to_integer(unsigned(d_in(3 downto 0)));
			if ((selectIndex >= 0) and (selectIndex <= 10)) then
				if (fft_ready = '1') then
					digit0 <= fftV(selectIndex)(3 downto 0);
					digit1 <= fftV(selectIndex)(7 downto 4);
					digit2 <= fftV(selectIndex)(11 downto 8);
					digit3 <= fftV(selectIndex)(15 downto 12);
					d_out <= fftV(selectIndex)(23 downto 16);
				end if;
			end if;				
		
			if (commandStateCurrent = commandStateNext) then
				case commandStateCurrent is
					when waitForCommand =>
						rt_packetTx <= '0';
						if ((fft_ready = '1') and (d_in(7) = '1')) then
							fftV(0) := fft_max_index;
							fftV(1) := fft0_re;
							fftV(2) := fft0_im;
							fftV(3) := fft1_re;
							fftV(4) := fft1_im;
							fftV(5) := fft2_re;
							fftV(6) := fft2_im;
							fftV(7) := fft3_re;
							fftV(8) := fft3_im;
							fftV(9) := fft4_re;
							fftV(10) := fft4_im;
							fftCount := x"0";
							fft_read <= '1';
							commandStateNext <= sendFFT;
						elsif (rt_packetRx = '1') then
							rt_packetRd <= '1';
							commandStateNext <= processCommand;
						else
							commandStateNext <= waitForCommand;
						end if;
					
					when sendFFT =>
						if (fft_ready = '0') then
							fft_read <= '0';
						end if;
						
						if (rt_packetSent = '1') then
							rt_commandTx <= x"8C"; --Report FFT
							rt_dataTx (7 downto 0) <= x"0" & std_logic_vector(fftCount);
							rt_dataTx (31 downto 8) <= fftV(to_integer(fftCount));
							rt_packetTx <= '1'; -- ready to transmit
							--fftDelayCounter := 0;
							--commandStateNext <= delayFFT;
							commandStateNext <= waitFFT;
						end if;
					
					when delayFFT =>
						fftDelayCounter := fftDelayCounter + 1;
						if (fftDelayCounter > 250000) then
							commandStateNext <= waitFFT;
						end if;
					
					when waitFFT =>
						if (rt_packetSent = '0') then
							rt_packetTx <= '0';
							if (fftCount = x"A") then
								commandStateNext <= waitForCommand;
							else
								fftCount := fftCount + x"1";
								commandStateNext <= sendFFT;
							end if;
						end if;
						
					when processCommand =>
						if (rt_packetSent = '1') then -- if a complete packet has been sent
							case rt_commandRx is
								when x"00" => -- Query Report Message Received
									commandStateNext <= waitForCommand;

								when x"82" => --Report Digital Inputs Message received
									case rt_dataRx (7 downto 0) is
										when x"00" => --Digital inputs PORT x"00"
											rt_commandTx <= x"82"; --Report Digital Inputs
											rt_dataTx (7 downto 0) <= x"00";
											rt_dataTx (15 downto 8) <= d_in;
											rt_dataTx (23 downto 16) <= x"00";
											rt_dataTx  (31 downto 24) <= x"00";
											rt_packetTx <= '1'; -- ready to transmit
											
										when others =>
											--Invalid Digital Input Port receieved
									end case;
									commandStateNext <= waitForCommand;
										
								when x"85" => --Report DC Offset
									case rt_dataRx (7 downto 0) is --Channel Select
										when x"00" =>
											rt_commandTx <= x"85"; --Report DC Offset
											rt_dataTx (7 downto 0) <= x"00"; --Channel
											rt_dataTx (23 downto 8) <= x"0" & dcOffset0;
											rt_dataTx  (31 downto 24) <= x"00";
											rt_packetTx <= '1'; -- ready to transmit
											
										when x"01" =>
											rt_commandTx <= x"85"; --Report DC Offset
											rt_dataTx (7 downto 0) <= x"01"; --Channel
											rt_dataTx (23 downto 8) <= x"0" & dcOffset1;
											rt_dataTx  (31 downto 24) <= x"00";
											rt_packetTx <= '1'; -- ready to transmit
											
										when x"02" =>
											rt_commandTx <= x"85"; --Report DC Offset
											rt_dataTx (7 downto 0) <= x"02"; --Channel
											rt_dataTx (23 downto 8) <= x"0" & dcOffset2;
											rt_dataTx  (31 downto 24) <= x"00";
											rt_packetTx <= '1'; -- ready to transmit
											
										when x"03" =>
											rt_commandTx <= x"85"; --Report DC Offset
											rt_dataTx (7 downto 0) <= x"03"; --Channel
											rt_dataTx (23 downto 8) <= x"0" & dcOffset3;
											rt_dataTx  (31 downto 24) <= x"00";
											rt_packetTx <= '1'; -- ready to transmit
											
										when x"04" =>
											rt_commandTx <= x"85"; --Report DC Offset
											rt_dataTx (7 downto 0) <= x"04"; --Channel
											rt_dataTx (23 downto 8) <= x"0" & dcOffset4;
											rt_dataTx  (31 downto 24) <= x"00";
											rt_packetTx <= '1'; -- ready to transmit
											
										when others =>
											--Invalid Channel Received
											
									end case;
									commandStateNext <= waitForCommand;
								
								when x"86" => --Report PkToPk
									case rt_dataRx (7 downto 0) is --Channel Select
										when x"00" =>
											rt_commandTx <= x"86"; --Report PkToPk
											rt_dataTx (7 downto 0) <= x"00"; --Channel
											rt_dataTx (15 downto 8) <= max0 (11 downto 4);
											rt_dataTx (23 downto 16) <= max0 (3 downto 0) & min0 (11 downto 8);
											rt_dataTx  (31 downto 24) <= min0 (7 downto 0);
											rt_packetTx <= '1'; -- ready to transmit
											
										when x"01" =>
											rt_commandTx <= x"86"; --Report PkToPk
											rt_dataTx (7 downto 0) <= x"01"; --Channel
											rt_dataTx (15 downto 8) <= max1 (11 downto 4);
											rt_dataTx (23 downto 16) <= max1 (3 downto 0) & min1 (11 downto 8);
											rt_dataTx  (31 downto 24) <= min1 (7 downto 0);
											rt_packetTx <= '1'; -- ready to transmit
										
										when x"02" =>
											rt_commandTx <= x"86"; --Report PkToPk
											rt_dataTx (7 downto 0) <= x"02"; --Channel
											rt_dataTx (15 downto 8) <= max2 (11 downto 4);
											rt_dataTx (23 downto 16) <= max2 (3 downto 0) & min2 (11 downto 8);
											rt_dataTx  (31 downto 24) <= min2 (7 downto 0);
											rt_packetTx <= '1'; -- ready to transmit
											
										when x"03" =>
											rt_commandTx <= x"86"; --Report PkToPk
											rt_dataTx (7 downto 0) <= x"03"; --Channel
											rt_dataTx (15 downto 8) <= max3 (11 downto 4);
											rt_dataTx (23 downto 16) <= max3 (3 downto 0) & min3 (11 downto 8);
											rt_dataTx  (31 downto 24) <= min3 (7 downto 0);
											rt_packetTx <= '1'; -- ready to transmit
											
										when x"04" =>
											rt_commandTx <= x"86"; --Report PkToPk
											rt_dataTx (7 downto 0) <= x"04"; --Channel
											rt_dataTx (15 downto 8) <= max4 (11 downto 4);
											rt_dataTx (23 downto 16) <= max4 (3 downto 0) & min4 (11 downto 8);
											rt_dataTx  (31 downto 24) <= min4 (7 downto 0);
											rt_packetTx <= '1'; -- ready to transmit
											
										when others =>
											--Invalid Channel Received
									end case;
									commandStateNext <= waitForCommand;
									
								when x"87" => --Report CH Timing
									rt_commandTx <= x"87"; --Report PkToPk
									case rt_dataRx (7 downto 0) is --Channel Select
										when x"00" =>
											rt_dataTx (7 downto 0) <= x"00"; --Channel
											rt_dataTx (23 downto 8) <= pingTime0 (15 downto 0);
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"01" =>
											rt_dataTx (7 downto 0) <= x"01"; --Channel
											rt_dataTx (23 downto 8) <= pingTime1 (15 downto 0);
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"02" =>
											rt_dataTx (7 downto 0) <= x"02"; --Channel
											rt_dataTx (23 downto 8) <= pingTime2 (15 downto 0);
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"03" =>
											rt_dataTx (7 downto 0) <= x"03"; --Channel
											rt_dataTx (23 downto 8) <= pingTime3 (15 downto 0);
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"04" =>
											rt_dataTx (7 downto 0) <= x"04"; --Channel
											rt_dataTx (23 downto 8) <= pingTime4 (15 downto 0);
											rt_dataTx (31 downto 24) <= x"00";
											
										when others =>
											--Invalid channel received
									end case;
									rt_packetTx <= '1'; -- ready to transmit
									commandStateNext <= waitForCommand;
									
								when x"89" => --ReportDPotValues
									rt_commandTx <= x"89";
									case rt_dataRx(7 downto 0) is --Channel
										when x"00" =>
											rt_dataTx (7 downto 0) <= x"00"; --Channel
											rt_dataTx (15 downto 8) <= vgnB0;
											rt_dataTx (23 downto 16) <= vgnA0;
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"01" =>
											rt_dataTx (7 downto 0) <= x"01"; --Channel
											rt_dataTx (15 downto 8) <= AdderOut0;
											rt_dataTx (23 downto 16) <= vref0;
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"02" =>
											rt_dataTx (7 downto 0) <= x"02"; --Channel
											rt_dataTx (15 downto 8) <= vgnB1;
											rt_dataTx (23 downto 16) <= vgnA1;
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"03" =>
											rt_dataTx (7 downto 0) <= x"03"; --Channel
											rt_dataTx (15 downto 8) <= AdderOut1;
											rt_dataTx (23 downto 16) <= vref1;
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"04" =>
											rt_dataTx (7 downto 0) <= x"04"; --Channel
											rt_dataTx (15 downto 8) <= vgnB2;
											rt_dataTx (23 downto 16) <= vgnA2;
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"05" =>
											rt_dataTx (7 downto 0) <= x"05"; --Channel
											rt_dataTx (15 downto 8) <= AdderOut2;
											rt_dataTx (23 downto 16) <= vref2;
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"06" =>
											rt_dataTx (7 downto 0) <= x"06"; --Channel
											rt_dataTx (15 downto 8) <= vgnB3;
											rt_dataTx (23 downto 16) <= vgnA3;
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"07" =>
											rt_dataTx (7 downto 0) <= x"07"; --Channel
											rt_dataTx (15 downto 8) <= AdderOut3;
											rt_dataTx (23 downto 16) <= vref3;
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"08" =>
											rt_dataTx (7 downto 0) <= x"08"; --Channel
											rt_dataTx (15 downto 8) <= vgnB4;
											rt_dataTx (23 downto 16) <= vgnA4;
											rt_dataTx (31 downto 24) <= x"00";
											
										when x"09" =>
											rt_dataTx (7 downto 0) <= x"09"; --Channel
											rt_dataTx (15 downto 8) <= AdderOut4;
											rt_dataTx (23 downto 16) <= vref4;
											rt_dataTx (31 downto 24) <= x"00";
										
										when others =>
											--Invalid pot channel
									end case;
									rt_packetTx <= '1'; -- ready to transmit
									commandStateNext <= waitForCommand;
								
								when x"8A" => --ReportSampleFreq
									rt_commandTx <= x"8A"; --ReportSampleFreq
									rt_dataTx (31 downto 0) <= sampleFrequency;
									rt_packetTx <= '1'; -- ready to transmit
									commandStateNext <= waitForCommand;
								
								when x"8B" => --ReportSamplePeriod
									rt_commandTx <= x"8B"; --ReportSamplePeriod
									rt_dataTx (11 downto 0) <= samplePeriod;
									rt_dataTx (31 downto 12) <= x"00000";
									rt_packetTx <= '1'; -- ready to transmit
									commandStateNext <= waitForCommand;
								
								when x"0B" => -- Set Servo Output message received
									commandStateNext <= waitForCommand;
								
								when x"0C" => -- set Digital output message received
									tempD_outPort := rt_dataRx (7 downto 0);
									tempD_outValue := rt_dataRx (15 downto 8);
									case tempD_outPort is
										when x"00" =>
											--d_out <= tempD_outValue;
											
										when others =>
											-- port not defined
									end case;
									commandStateNext <= waitForCommand;
								
								when x"0D" => --Ping Message Received
									rt_commandTx <= x"84"; --Pong Command to be sent in respone
									rt_dataTx (7 downto 0) <= x"50";   --P
									rt_dataTx (15 downto 8) <= x"4F";  --O
									rt_dataTx (23 downto 16) <= x"4E"; --N
									rt_dataTx  (31 downto 24) <= x"47";--G
									rt_packetTx <= '1'; -- ready to transmit
									commandStateNext <= waitForCommand;
								
								when x"0F" => --Set Pinger Frequency Received
									max268Interface_freqValueIn <= rt_dataRx(15 downto 0);
									Q <= rt_dataRx(30 downto 24);
									commandStateNext <= waitForCommand;
								
								when x"10" => --Set APL Message Received
									case rt_dataRx(7 downto 0) is
										when x"00" => --PkToPkDecay
											pkToPkDecayValue(23 downto 16) <= x"00";
											pkToPkDecayValue(15 downto 0) <= rt_dataRx(23 downto 8);
											
										when x"01" => --DcCalcDecay
											dcOffsetIncrement(23 downto 16) <= x"00";
											dcOffsetIncrement(15 downto 0) <= rt_dataRx(23 downto 8);
										
										when x"02" => --MinPingThreshold
											pingDetectMinPingThreshold <= rt_dataRx(19 downto 8);
										
										when x"03" => --PingThresholdRelative
											pingDetectAbsoluteFlag <= '0';
											pingDetectThresholdRelative <= rt_dataRx(10 downto 8);
										
										when x"04" => --PingThresholdAbsMax
											pingDetectAbsoluteFlag <= '1';
											pingDetectThresholdAbsMax <= rt_dataRx(19 downto 8);
											pingDetectThresholdAbsFromDC <= x"000";
										
										when x"05" => --PingThresholdAbsMin
											pingDetectAbsoluteFlag <= '1';
											pingDetectThresholdAbsMin <= rt_dataRx(19 downto 8);
											pingDetectThresholdAbsFromDC <= x"000";
										
										when x"06" => --PingThresholdAbsFromDC
											pingDetectAbsoluteFlag <= '1';
											pingDetectThresholdAbsFromDC <= rt_dataRx(19 downto 8);
											pingDetectThresholdAbsMin <= x"000";
											pingDetectThresholdAbsMax <= x"000";
											
										when x"07" => --SilentTime
											pingDetectSilentTime <= rt_dataRx(19 downto 8);
											
										when x"08" => --PingCount
											pingDetectPingCount <= rt_dataRx(19 downto 8);
											
										when x"09" => --RawMode
											rawMode <= rt_dataRx(9 downto 8);
											
										when x"0A" => --RawDataMode
											rawDataMode <= rt_dataRx(8);
											
										when x"0B" => --ArbiterMode
											arbiterMode <= rt_dataRx(9 downto 8);
										
										when x"0C" => --VariableGain
											vgnM <= rt_dataRx(15 downto 8);
										
										when others =>
											--Invalid configuration field received
									end case;
									commandStateNext <= waitForCommand;
								
								when x"8F" => --Report MCU ID
									rt_commandTx <= x"8F"; --MCU ID to be sent in respone
									rt_dataTx (7 downto 0) <= x"46";   --F
									rt_dataTx (15 downto 8) <= x"50";  --P
									rt_dataTx (23 downto 16) <= x"47"; --G
									rt_dataTx  (31 downto 24) <= x"41";--A
									rt_packetTx <= '1'; -- ready to transmit
									commandStateNext <= waitForCommand;
								
								when others =>
									commandStateNext <= waitForCommand;
									
							end case;
						else
							rt_packetTx <= '0';
						end if;
					
				end case;
			end if;
		end if;
	end process;

end Behavioral;

