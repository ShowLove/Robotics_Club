--This is the reference compoent for the Nexy2 FPGa seven segment display
--4 User inputs are required for the 4 displays. 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

package SevenSegment is
	component SegmentRefComp is
	Port    (    
		CLK 			: in std_logic; 
		
		-- Four user inputs
		input1      : in std_logic_vector(3 downto 0):= "0000";
		input2      : in std_logic_vector(3 downto 0):= "0000";
		input3      : in std_logic_vector(3 downto 0):= "0000";
		input4      : in std_logic_vector(3 downto 0) := "0000";
		
		--Actual seven segment display and anode 
		sevenseg1   : out std_logic_vector(6 downto 0);
		anodes	   : out std_logic_vector(3 downto 0)
	);
	end component SegmentRefComp;
end package SevenSegment;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity SegmentRefComp is
  Port    (    
			CLK 			: in std_logic; 
			
			-- Four user inputs
			input1      : in std_logic_vector(3 downto 0):= "0000";
			input2      : in std_logic_vector(3 downto 0):= "0000";
			input3      : in std_logic_vector(3 downto 0):= "0000";
			input4      : in std_logic_vector(3 downto 0) := "0000";
			
			--Actual seven segment display and anode 
			sevenseg1   : out std_logic_vector(6 downto 0);
			anodes	   : out std_logic_vector(3 downto 0)
          );

end SegmentRefComp ;

architecture display of SegmentRefComp is

begin
	process (clk)
		variable counter : integer;
		variable digit : integer;
	begin
		if (rising_edge(clk)) then
			case digit is
				when 1 =>				-- Display the first digit 
					anodes <= "1110";
					CASE input1 IS
						WHEN x"0" =>
							sevenseg1 <="1000000";
						when x"1" =>	
							sevenseg1 <="1111001";
						when x"2" =>
							sevenseg1 <="0100100"; 
						when x"3" =>
							sevenseg1 <="0110000"; 
						when x"4" =>
							sevenseg1 <="0011001"; 
						when x"5" =>
							sevenseg1 <="0010010"; 
						when x"6" =>
							sevenseg1 <="0000010"; 
						when x"7" =>
							sevenseg1 <="1111000"; 
						when x"8" =>
							sevenseg1 <="0000000";
						when x"9" =>
							sevenseg1 <="0010000";
						when x"A" =>
							sevenseg1 <="0001000";
						when x"B" =>
							sevenseg1 <="0000011";
						when x"C" =>
							sevenseg1 <="1000110";
						when x"D" =>
							sevenseg1 <="0100001";
						when x"E" =>
							sevenseg1 <="0000110";
						when others =>
							sevenseg1 <="0001110"; 
					END CASE;
				when 2 =>			-- Display the second digit 
					anodes <= "1101";
					CASE input2 IS
						WHEN x"0" =>
							sevenseg1 <="1000000";
						when x"1" =>	
							sevenseg1 <="1111001";
						when x"2" =>
							sevenseg1 <="0100100"; 
						when x"3" =>
							sevenseg1 <="0110000"; 
						when x"4" =>
							sevenseg1 <="0011001"; 
						when x"5" =>
							sevenseg1 <="0010010"; 
						when x"6" =>
							sevenseg1 <="0000010"; 
						when x"7" =>
							sevenseg1 <="1111000"; 
						when x"8" =>
							sevenseg1 <="0000000";
						when x"9" =>
							sevenseg1 <="0010000";
						when x"A" =>
							sevenseg1 <="0001000";
						when x"B" =>
							sevenseg1 <="0000011";
						when x"C" =>
							sevenseg1 <="1000110";
						when x"D" =>
							sevenseg1 <="0100001";
						when x"E" =>
							sevenseg1 <="0000110";
						when others =>
							sevenseg1 <="0001110";  
					END CASE;
				when 3 =>			-- Display the third digit 
					anodes <= "1011";
					CASE input3 IS
						WHEN x"0" =>
							sevenseg1 <="1000000";
						when x"1" =>	
							sevenseg1 <="1111001";
						when x"2" =>
							sevenseg1 <="0100100"; 
						when x"3" =>
							sevenseg1 <="0110000"; 
						when x"4" =>
							sevenseg1 <="0011001"; 
						when x"5" =>
							sevenseg1 <="0010010"; 
						when x"6" =>
							sevenseg1 <="0000010"; 
						when x"7" =>
							sevenseg1 <="1111000"; 
						when x"8" =>
							sevenseg1 <="0000000";
						when x"9" =>
							sevenseg1 <="0010000";
						when x"A" =>
							sevenseg1 <="0001000";
						when x"B" =>
							sevenseg1 <="0000011";
						when x"C" =>
							sevenseg1 <="1000110";
						when x"D" =>
							sevenseg1 <="0100001";
						when x"E" =>
							sevenseg1 <="0000110";
						when others =>
							sevenseg1 <="0001110";  
					END CASE;
				when 4 =>			-- Display the fourth digit 
					anodes <= "0111";
					CASE input4 IS
						WHEN x"0" =>
							sevenseg1 <="1000000";
						when x"1" =>	
							sevenseg1 <="1111001";
						when x"2" =>
							sevenseg1 <="0100100"; 
						when x"3" =>
							sevenseg1 <="0110000"; 
						when x"4" =>
							sevenseg1 <="0011001"; 
						when x"5" =>
							sevenseg1 <="0010010"; 
						when x"6" =>
							sevenseg1 <="0000010"; 
						when x"7" =>
							sevenseg1 <="1111000"; 
						when x"8" =>
							sevenseg1 <="0000000";
						when x"9" =>
							sevenseg1 <="0010000";
						when x"A" =>
							sevenseg1 <="0001000";
						when x"B" =>
							sevenseg1 <="0000011";
						when x"C" =>
							sevenseg1 <="1000110";
						when x"D" =>
							sevenseg1 <="0100001";
						when x"E" =>
							sevenseg1 <="0000110";
						when others =>
							sevenseg1 <="0001110";  
					END CASE;
				when others =>
					--Do nothing, incorrect value
			end case;
			
			-- Counter with 50Mhz FPGA clock => 1/50MHz = 20 ns
			-- 0.001/20ns = 5000 counts
			if (counter > 50000) then
				counter := 0;
				if (digit > 4) then
					digit := 0;
				end if;
				digit := digit + 1;
			end if;
			counter := counter + 1;
		end if;
	end process;
			

end display;
