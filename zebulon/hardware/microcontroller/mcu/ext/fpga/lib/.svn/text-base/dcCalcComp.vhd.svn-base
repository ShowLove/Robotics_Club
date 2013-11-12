library ieee;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

package DCCalc is
	component dcCalcComp is
	port (	
		clk : in std_logic;
		
		analog : in std_logic_vector (11 downto 0);
		dc : out std_logic_vector (11 downto 0);
		
		analogReady : in std_logic;
		incrementValue : in std_logic_vector  (23 downto 0)
	);
	end component dcCalcComp;
end package DCCalc;

library ieee;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

entity dcCalcComp is
	port (
		clk : in std_logic;
		
		analog : in std_logic_vector (11 downto 0);
		dc : out std_logic_vector (11 downto 0);
		
		analogReady : in std_logic;
		incrementValue : in std_logic_vector  (23 downto 0) := x"000000"
	);
end entity dcCalcComp;


architecture Behavioral of dcCalcComp is
	signal dcOffset : std_logic_vector (23 downto 0) := x"800000";
begin

	process(clk)
		variable doneFlag : std_logic := '0';
		
	begin
		if (rising_edge(clk)) then
			if ((analogReady = '1') and (doneFlag = '0')) then
				if (dcOffset(23 downto 12) < analog) then
					dcOffset <= dcOffset + incrementValue;
				elsif (dcOffset(23 downto 12) > analog) then
					dcOffset <= dcOffset - incrementValue;
				end if;
				dc <= dcOffset (23 downto 12);
				doneFlag := '1';
			elsif (analogReady = '0') then
				doneFlag := '0';				
			end if;
		end if;
	end process;

end architecture Behavioral;
