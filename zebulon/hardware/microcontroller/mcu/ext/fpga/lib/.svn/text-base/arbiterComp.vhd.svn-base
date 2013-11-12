library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;
use IEEE.MATH_REAL.all;
use IEEE.math_complex.all;

package arbiter is
	component arbiterComp is
	port (	
		clock : in std_logic;
		
		dcCalc_dc0 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue0 : in std_logic_vector (11 downto 0);
		pkToPk_minValue0 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut0 : in std_logic;
		vgnA0 : out std_logic_vector (7 downto 0) := x"15";
		vgnB0 : out std_logic_vector (7 downto 0) := x"15";
		vref0 : out std_logic_vector (7 downto 0) := x"7F";
		
		dcCalc_dc1 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue1 : in std_logic_vector (11 downto 0);
		pkToPk_minValue1 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut1 : in std_logic;
		vgnA1 : out std_logic_vector (7 downto 0) := x"15";
		vgnB1 : out std_logic_vector (7 downto 0) := x"15";
		vref1 : out std_logic_vector (7 downto 0) := x"7F";

		dcCalc_dc2 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue2 : in std_logic_vector (11 downto 0);
		pkToPk_minValue2 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut2 : in std_logic;
		vgnA2 : out std_logic_vector (7 downto 0) := x"15";
		vgnB2 : out std_logic_vector (7 downto 0) := x"15";
		vref2 : out std_logic_vector (7 downto 0) := x"7F";

		dcCalc_dc3 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue3 : in std_logic_vector (11 downto 0);
		pkToPk_minValue3 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut3 : in std_logic;
		vgnA3 : out std_logic_vector (7 downto 0) := x"15";
		vgnB3 : out std_logic_vector (7 downto 0) := x"15";
		vref3 : out std_logic_vector (7 downto 0) := x"7F";

		dcCalc_dc4 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue4 : in std_logic_vector (11 downto 0);
		pkToPk_minValue4 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut4 : in std_logic;
		vgnA4 : out std_logic_vector (7 downto 0) := x"15";
		vgnB4 : out std_logic_vector (7 downto 0) := x"15";
		vref4 : out std_logic_vector (7 downto 0) := x"7F";
		
		mode : in std_logic_vector (1 downto 0) := "00";
		vgnM : in std_logic_vector (7 downto 0) := x"15"
	);
	end component arbiterComp;
end package arbiter;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;
use IEEE.MATH_REAL.all;
use IEEE.math_complex.all;

entity arbiterComp is
		port (	
		clock : in std_logic;
		
		dcCalc_dc0 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue0 : in std_logic_vector (11 downto 0);
		pkToPk_minValue0 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut0 : in std_logic;
		vgnA0 : out std_logic_vector (7 downto 0) := x"15";
		vgnB0 : out std_logic_vector (7 downto 0) := x"15";
		vref0 : out std_logic_vector (7 downto 0) := x"7F";
		
		dcCalc_dc1 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue1 : in std_logic_vector (11 downto 0);
		pkToPk_minValue1 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut1 : in std_logic;
		vgnA1 : out std_logic_vector (7 downto 0) := x"15";
		vgnB1 : out std_logic_vector (7 downto 0) := x"15";
		vref1 : out std_logic_vector (7 downto 0) := x"7F";

		dcCalc_dc2 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue2 : in std_logic_vector (11 downto 0);
		pkToPk_minValue2 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut2 : in std_logic;
		vgnA2 : out std_logic_vector (7 downto 0) := x"15";
		vgnB2 : out std_logic_vector (7 downto 0) := x"15";
		vref2 : out std_logic_vector (7 downto 0) := x"7F";

		dcCalc_dc3 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue3 : in std_logic_vector (11 downto 0);
		pkToPk_minValue3 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut3 : in std_logic;
		vgnA3 : out std_logic_vector (7 downto 0) := x"15";
		vgnB3 : out std_logic_vector (7 downto 0) := x"15";
		vref3 : out std_logic_vector (7 downto 0) := x"7F";

		dcCalc_dc4 : in std_logic_vector (11 downto 0);
		pkToPk_maxValue4 : in std_logic_vector (11 downto 0);
		pkToPk_minValue4 : in std_logic_vector (11 downto 0);
		pingDetect_pingOut4 : in std_logic;
		vgnA4 : out std_logic_vector (7 downto 0) := x"15";
		vgnB4 : out std_logic_vector (7 downto 0) := x"15";
		vref4 : out std_logic_vector (7 downto 0) := x"7F";
		
		mode : in std_logic_vector (1 downto 0) := "00";
		vgnM : in std_logic_vector (7 downto 0) := x"15"
	);
end arbiterComp;

architecture Behavioral of arbiterComp is

begin

	process(clock)
		variable vgnA : std_logic_vector (7 downto 0) := x"15";
		variable vgnB : std_logic_vector (7 downto 0) := x"15";
		
		variable vgnA_0 : std_logic_vector (7 downto 0) := x"15";
		variable vgnB_0 : std_logic_vector (7 downto 0) := x"15";
		variable vgnA_1 : std_logic_vector (7 downto 0) := x"15";
		variable vgnB_1 : std_logic_vector (7 downto 0) := x"15";
		variable vgnA_2 : std_logic_vector (7 downto 0) := x"15";
		variable vgnB_2 : std_logic_vector (7 downto 0) := x"15";
		variable vgnA_3 : std_logic_vector (7 downto 0) := x"15";
		variable vgnB_3 : std_logic_vector (7 downto 0) := x"15";
		variable vgnA_4 : std_logic_vector (7 downto 0) := x"15";
		variable vgnB_4 : std_logic_vector (7 downto 0) := x"15";
		
		variable pkToPk0 : std_logic_vector (11 downto 0) := x"000";
		variable pkToPk1 : std_logic_vector (11 downto 0) := x"000";
		variable pkToPk2 : std_logic_vector (11 downto 0) := x"000";
		variable pkToPk3 : std_logic_vector (11 downto 0) := x"000";
		variable pkToPk4 : std_logic_vector (11 downto 0) := x"000";
		variable maxPktoPk : std_logic_vector (11 downto 0) := x"000";
		
		variable counter : integer := 0;
		
		variable done : std_logic := '0';
		
		variable currentGain : std_logic_vector(7 downto 0) := x"2A";
		
		variable currentGain0 : std_logic_vector(7 downto 0) := x"2A";
		variable currentGain1 : std_logic_vector(7 downto 0) := x"2A";
		variable currentGain2 : std_logic_vector(7 downto 0) := x"2A";
		variable currentGain3 : std_logic_vector(7 downto 0) := x"2A";
		variable currentGain4 : std_logic_vector(7 downto 0) := x"2A";
		
	begin
		if (rising_edge(clock)) then
			counter := counter + 1;
			done := '0';
			if (counter > 50000000) then -- 1 second has elapsed
				counter := 0;
			
				pkToPk0 := pkToPk_maxValue0 - pkToPk_minValue0;
				pkToPk1 := pkToPk_maxValue1 - pkToPk_minValue1;
				pkToPk2 := pkToPk_maxValue2 - pkToPk_minValue2;
				pkToPk3 := pkToPk_maxValue3 - pkToPk_minValue3;
				pkToPk4 := pkToPk_maxValue4 - pkToPk_minValue4;
				
				maxPkToPk := pkToPk0;
				
				if (pkToPk1 > maxPkToPk) then
					maxPkToPk := pkToPk1;
				end if;
				if (pkToPk2 > maxPkToPk) then
					maxPkToPk := pkToPk2;
				end if;
				if (pkToPk3 > maxPkToPk) then
					maxPkToPk := pkToPk3;
				end if;
				if (pkToPk4 > maxPkToPk) then
					maxPkToPk := pkToPk4;
				end if;
				
				if (maxPkToPk < x"9B1") then
					currentGain := currentGain + x"01";
				else
					currentGain := currentGain - x"01";
				end if;
				
				if (currentGain < x"28") then
					currentGain := x"28";
				elsif (currentGain > x"F4") then
					currentGain := x"F4";
				end if;
				
				vgnA := "0" & currentGain(7 downto 1);
				vgnB := ("0" & currentGain(7 downto 1)) + currentGain(0);				
				
				-----------------------------
				if (pkToPk0 < x"9B1") then
					currentGain0 := currentGain0 + x"01";
				else
					currentGain0 := currentGain0 - x"01";
				end if;
				if (currentGain0 < x"28") then
					currentGain0 := x"28";
				elsif (currentGain0 > x"F4") then
					currentGain0 := x"F4";
				end if;
				vgnA_0 := "0" & currentGain0(7 downto 1);
				vgnB_0 := ("0" & currentGain0(7 downto 1)) + currentGain0(0);				
				
				if (pkToPk1 < x"9B1") then
					currentGain1 := currentGain1 + x"01";
				else
					currentGain1 := currentGain1 - x"01";
				end if;
				if (currentGain1 < x"28") then
					currentGain1 := x"28";
				elsif (currentGain1 > x"F4") then
					currentGain1 := x"F4";
				end if;
				vgnA_1 := "0" & currentGain1(7 downto 1);
				vgnB_1 := ("0" & currentGain1(7 downto 1)) + currentGain1(0);				
				
				if (pkToPk2 < x"9B1") then
					currentGain2 := currentGain2 + x"01";
				else
					currentGain2 := currentGain2 - x"01";
				end if;
				if (currentGain2 < x"28") then
					currentGain2 := x"28";
				elsif (currentGain2 > x"F4") then
					currentGain2 := x"F4";
				end if;
				vgnA_2 := "0" & currentGain2(7 downto 1);
				vgnB_2 := ("0" & currentGain2(7 downto 1)) + currentGain2(0);				
				
				if (pkToPk3 < x"9B1") then
					currentGain3 := currentGain3 + x"01";
				else
					currentGain3 := currentGain3 - x"01";
				end if;
				if (currentGain3 < x"28") then
					currentGain3 := x"28";
				elsif (currentGain3 > x"F4") then
					currentGain3 := x"F4";
				end if;
				vgnA_3 := "0" & currentGain3(7 downto 1);
				vgnB_3 := ("0" & currentGain3(7 downto 1)) + currentGain3(0);				
				
				if (pkToPk4 < x"9B1") then
					currentGain4 := currentGain4 + x"01";
				else
					currentGain4 := currentGain4 - x"01";
				end if;
				if (currentGain4 < x"28") then
					currentGain4 := x"28";
				elsif (currentGain4 > x"F4") then
					currentGain4 := x"F4";
				end if;
				vgnA_4 := "0" & currentGain4(7 downto 1);
				vgnB_4 := ("0" & currentGain4(7 downto 1)) + currentGain4(0);				
				
				done := '1';
				
				if (done = '1') then
					case mode is
						when "00" =>
							vgnA0 <= vgnA;
							vgnB0 <= vgnB;
							vref0 <= x"7F";
							
							vgnA1 <= vgnA;
							vgnB1 <= vgnB;
							vref1 <= x"7F";
							
							vgnA2 <= vgnA;
							vgnB2 <= vgnB;
							vref2 <= x"7F";
							
							vgnA3 <= vgnA;
							vgnB3 <= vgnB;
							vref3 <= x"7F";
							
							vgnA4 <= vgnA;
							vgnB4 <= vgnB;
							vref4 <= x"7F";
						when "01" =>
							vgnA0 <= vgnA_0;
							vgnB0 <= vgnB_0;
							vref0 <= x"7F";
							
							vgnA1 <= vgnA_1;
							vgnB1 <= vgnB_1;
							vref1 <= x"7F";
							
							vgnA2 <= vgnA_2;
							vgnB2 <= vgnB_2;
							vref2 <= x"7F";
							
							vgnA3 <= vgnA_3;
							vgnB3 <= vgnB_3;
							vref3 <= x"7F";
							
							vgnA4 <= vgnA_4;
							vgnB4 <= vgnB_4;
							vref4 <= x"7F";
						when "10" =>
							vgnA0 <= vgnM;
							vgnB0 <= vgnM;
							vref0 <= x"7F";
							
							vgnA1 <= vgnM;
							vgnB1 <= vgnM;
							vref1 <= x"7F";
							
							vgnA2 <= vgnM;
							vgnB2 <= vgnM;
							vref2 <= x"7F";
							
							vgnA3 <= vgnM;
							vgnB3 <= vgnM;
							vref3 <= x"7F";
							
							vgnA4 <= vgnM;
							vgnB4 <= vgnM;
							vref4 <= x"7F";
							
						when others =>
							--Invalid mode
					end case;
						

				end if;
					
			end if;
			
		end if;
	end process;
end Behavioral;