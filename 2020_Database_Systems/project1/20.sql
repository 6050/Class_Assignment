SELECT name AS '상록시티 출신 트레이너의 이름', COUNT(*) AS '잡은 포켓몬 수'
FROM Trainer AS T, CatchedPokemon AS C
WHERE T.id = owner_id AND hometown = 'Sangnok City'
GROUP BY name
ORDER BY COUNT(*);