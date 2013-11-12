library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

package PingDetect is
	component ping_detect is
	port(
		clk	: in std_logic;
		DcOffset : in std_logic_vector(11 downto 0);
		AnalogIn	: in std_logic_vector(11 downto 0);
		AnalogDone : in std_logic; -- std_logic_vector or std_logic
		SilentTime : in std_logic_vector	(11 downto 0);
		PingCount  : in std_logic_vector (11 downto 0);
		Max : in std_logic_vector (11 downto 0);
		Min : in std_logic_vector (11 downto 0);
		
		MinPingThreshold : in std_logic_vector (11 downto 0) := x"300";
		AbsoluteFlag : in std_logic;
		ThresholdRelative : in std_logic_vector (2 downto 0);
		ThresholdAbsMax : in std_logic_vector (11 downto 0);
		ThresholdAbsMin : in std_logic_vector (11 downto 0);
		ThresholdAbsFromDC : in std_logic_vector (11 downto 0);
		
		pingOut	  : out std_logic
	 );
	end component ping_detect;
end package PingDetect;


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity ping_detect is
port(
		clk	: in std_logic;
		DcOffset : in std_logic_vector(11 downto 0);
		AnalogIn	: in std_logic_vector(11 downto 0);
		AnalogDone : in std_logic; -- std_logic_vector or std_logic
		SilentTime : in std_logic_vector	(11 downto 0);
		PingCount  : in std_logic_vector (11 downto 0);
		Max : in std_logic_vector (11 downto 0);
		Min : in std_logic_vector (11 downto 0);

		MinPingThreshold : in std_logic_vector (11 downto 0) := x"300";
		AbsoluteFlag : in std_logic;
		ThresholdRelative : in std_logic_vector (2 downto 0);
		ThresholdAbsMax : in std_logic_vector (11 downto 0);
		ThresholdAbsMin : in std_logic_vector (11 downto 0);
		ThresholdAbsFromDC : in std_logic_vector (11 downto 0);
		
		pingOut	  : out std_logic
	 );
end ping_detect;

architecture Behavioral of ping_detect is

begin

	process(clk)
		type states is (listeningForSilence, inSilence, pingDetected);
		variable state : states := listeningForSilence; 
		variable	nextState : states := listeningForSilence;
		variable doneFlag : std_logic :='0';
		variable output :	std_logic := '0';
		variable count	: std_logic_vector(11 downto 0) := x"000";
		variable LeadWaveCount : integer := 0;
		variable tempTime : std_logic_vector (11 downto 0) := x"0FF";
		variable Threshold  : std_logic_vector (11 downto 0);
		variable thresholdHigh : std_logic_vector (11 downto 0);
		variable thresholdLow : std_logic_vector (11 downto 0);
	begin
		if(rising_edge(clk)) then
		
			if(AbsoluteFlag = '1') then
			
				if(ThresholdAbsFromDC = x"000") then
				
					thresholdHigh := ThresholdAbsMax;
					thresholdLow := ThresholdAbsMin;
				
				else
				
					if((x"FFF" - Dcoffset) > ThresholdAbsFromDC) then
						thresholdHigh := Dcoffset + ThresholdAbsFromDC;
					else
						thresholdHigh := x"FFF";
					end if;
						
					if((Dcoffset - x"000") > ThresholdAbsFromDC) then
						thresholdLow := DcOffset - ThresholdAbsFromDC;
					else
						thresholdLow := x"000";
					end if;
				
				end if;
			
			else
			
				Threshold := Max - Min;
			
				case ThresholdRelative is
					when "000" =>
						Threshold := Threshold(11 downto 0);
						
					when "001" =>
						Threshold := "0" & Threshold(11 downto 1);
					
					when "010" =>
						Threshold := "00" & Threshold(11 downto 2);
					
					when "011" =>
						Threshold := "000" & Threshold(11 downto 3);
					
					when "100" =>
						Threshold := "0000" & Threshold(11 downto 4);
					
					when "101" =>
						Threshold := "00000" & Threshold(11 downto 5);
					
					when "110" =>
						Threshold := "000000" & Threshold(11 downto 6);
					
					when "111" =>
						Threshold := "0000000" & Threshold(11 downto 7);
						
					when others =>
						--invalid mode
				end case;
			
				if((x"FFF" - Dcoffset) > Threshold) then
					thresholdHigh := Dcoffset + Threshold;
				else
					thresholdHigh := x"FFF";
				end if;
					
				if((Dcoffset - x"000") > Threshold) then
					thresholdLow := DcOffset - Threshold;
				else
					thresholdLow := x"000";
				end if;
			
			end if;
			
		
			if((AnalogDone = '1') and (doneFlag = '0') and ((Max-Min) > MinPingThreshold))then
				state := nextState;
				case (state) is
					when listeningForSilence =>
						pingOut <= '0';
						LeadWaveCount := 0;
						if(tempTime > x"000") then
							if((AnalogIn < thresholdHigh) and (AnalogIn > thresholdLow)) then
								tempTime := tempTime - x"001";
							else
								tempTime := silentTime;
							end if;
						else
							nextState := inSilence;
						end if;
						
					when inSilence	=>
						if((AnalogIn > thresholdHigh) or (AnalogIn < thresholdLow)) then
							count := count + x"001";
							if(count > PingCount) then
								nextState := pingDetected;
							end if;
						else
							
						end if;
						
					when pingDetected	 =>
						pingOut <= '1';
						LeadWaveCount := LeadWaveCount + 1;
						if(LeadWaveCount > 2048) then
							nextState := listeningForSilence;
						end if;
						tempTime := silentTime;
						count := x"000";
					
				end case;

				doneFlag := '1';
			
			elsif (AnalogDone = '0') then
				doneFlag := '0';		
			end if;
		
		end if;
	end process;
end Behavioral;

