library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

package channel is
	component channelComp is
	port (	
		clock : in std_logic;
		
		analogIn : in std_logic_vector (11 downto 0);
		analogDone : in std_logic;
		
		dcCalc_incrementValue : in std_logic_vector (23 downto 0);
		dcCalc_dc : buffer std_logic_vector (11 downto 0);
		
		pkToPk_decayValue : in std_logic_vector (23 downto 0);
		pkToPk_maxValue : out std_logic_vector (11 downto 0);
		pkToPk_minValue : out std_logic_vector (11 downto 0);
		
		pingDetect_silentTime : in std_logic_vector (11 downto 0);
		pingDetect_pingCount : in std_logic_vector (11 downto 0);
		pingDetect_minPingThreshold : in std_logic_vector (11 downto 0);
		pingDetect_absoluteFlag : std_logic;
		pingDetect_thresholdRelative : std_logic_vector (2 downto 0);
		pingDetect_thresholdAbsMax : std_logic_vector (11 downto 0);
		pingDetect_thresholdAbsMin : std_logic_vector (11 downto 0);
		pingDetect_thresholdAbsFromDC : std_logic_vector (11 downto 0);
		
		pingDetect_pingOut : out std_logic;
		
		adder_value : buffer std_logic_vector (7 downto 0);
		adder_done : out std_logic		
	);
	end component channelComp;
end package channel;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

use work.DCCalc.all;
use work.PkToPk.all;
use work.PingDetect.all;
use work.Adder.all;

entity channelComp is
	port (	
		clock : in std_logic;
		
		analogIn : in std_logic_vector (11 downto 0);
		analogDone : in std_logic;
		
		dcCalc_incrementValue : in std_logic_vector (23 downto 0);
		dcCalc_dc : buffer std_logic_vector (11 downto 0);
		
		pkToPk_decayValue : in std_logic_vector (23 downto 0);
		pkToPk_maxValue : buffer std_logic_vector (11 downto 0);
		pkToPk_minValue : buffer std_logic_vector (11 downto 0);
		
		pingDetect_silentTime : in std_logic_vector (11 downto 0);
		pingDetect_pingCount : in std_logic_vector (11 downto 0);
		pingDetect_minPingThreshold : in std_logic_vector (11 downto 0);
		pingDetect_absoluteFlag : std_logic;
		pingDetect_thresholdRelative : std_logic_vector (2 downto 0);
		pingDetect_thresholdAbsMax : std_logic_vector (11 downto 0);
		pingDetect_thresholdAbsMin : std_logic_vector (11 downto 0);
		pingDetect_thresholdAbsFromDC : std_logic_vector (11 downto 0);
		
		pingDetect_pingOut : out std_logic;
		
		adder_value : buffer std_logic_vector (7 downto 0);
		adder_done : out std_logic		
	);
end channelComp;

architecture Behavioral of channelComp is
begin
	
	dcCalc0: dcCalcComp port map
	(
		clk => clock,
		analog => analogIn,
		dc => dcCalc_dc,
		analogReady => analogDone,
		incrementValue => dcCalc_incrementValue
	);
	
	pkToPk0: pkToPkComp port map
	(
		clk => clock,
		analog => analogIn,
		maxValue => pkToPk_maxValue,
		minValue => pkToPk_minValue,		
		analogReady => analogDone,
		decayValue => pkToPk_decayValue
	);
	
	pingDetect0: ping_detect port map
	(
		clk => clock,
		DcOffset => dcCalc_dc,
		AnalogIn	=> analogIn,
		AnalogDone => analogDone,
		SilentTime => pingDetect_silentTime,
		PingCount => pingDetect_pingCount,
		Max => pkToPk_maxValue,
		Min => pkToPk_minValue,
		
		MinPingThreshold => pingDetect_minPingThreshold,

		AbsoluteFlag => pingDetect_absoluteFlag,	
		ThresholdRelative => pingDetect_thresholdRelative,
		ThresholdAbsMax => pingDetect_thresholdAbsMax,
		ThresholdAbsMin => pingDetect_thresholdAbsMin,
		ThresholdAbsFromDC => pingDetect_thresholdAbsFromDC,
		
		pingOut => pingDetect_pingOut
	);
	
	adder0: adderComp port map
	(
		clk => clock,
		dc => dcCalc_dc,
		AdderOut => adder_value,
		done => adder_done
	);
			
end Behavioral;

