library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

package Adder is
	component adderComp is
	port(
		clk : in std_logic;
		dc	 : in std_logic_vector(11 downto 0);
		
		AdderOut : buffer std_logic_vector(7 downto 0);
		done : out std_logic
	 );
	end component adderComp;
end package Adder;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity adderComp is
port(
		clk : in std_logic;
		dc	 : in std_logic_vector(11 downto 0);
		
		AdderOut : buffer std_logic_vector(7 downto 0);
		done : out std_logic
	 );
end adderComp;


architecture Behavioral of adderComp is

begin
	
	process(clk)
		variable counter : integer := 0;
		variable tempDC : std_logic_vector(7 downto 0);
	begin
		if (rising_edge(clk)) then
			counter := counter + 1;
			if (counter > 50000000) then --One second has elapsed
				counter := 0;
				tempDC := dc (11 downto 4);
				
				if (tempDC > x"80") then
					tempDC := tempDC - x"80";
					if (tempDC > AdderOut) then
						AdderOut <= x"00";
					else
						AdderOut <= AdderOut - tempDC;
					end if;
					done <= '1';
				else
					tempDC := x"80" - tempDC;
					if (tempDC > (x"FF" - AdderOut)) then
						AdderOut <= x"FF";
					else
						AdderOut <= AdderOut + tempDC;
					end if;
					done <= '1';
				end if;
				
			else
				done <= '0';
			end if;
		end if;
	end process;

end Behavioral;

